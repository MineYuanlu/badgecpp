#include "badgecpp/icons.hpp"
#include "badgecpp/b64.hpp"
#include "badgecpp/bbox.hpp"
#include "badgecpp/color.hpp"
#include "badgecpp/resources.h"
#include "badgecpp/version.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
namespace {
    const std::string data_url_prefix = "data:image/svg+xml;base64,";

    constexpr std::string_view BUILTIN_REPLACE_TAG = "REPLACE=\"TAG\"";

    /// @brief 将src中的tag替换为data, 假设 tag 出现在 src 中出现且只出现一次
    auto replace_tag(std::string_view src, size_t pos, std::string_view tag, const std::string &data) {
        std::string result;
        result.resize(src.size() - tag.size() + data.size());
        char *dst = result.data();

        // 复制 src 中 tag 之前的部分
        if (pos > 0) {
            std::memcpy(dst, src.data(), pos);
            dst += pos;
        }
        // 复制 data
        if (!data.empty()) {
            std::memcpy(dst, data.data(), data.size());
            dst += data.size();
        }
        // 复制 src 中 tag 之后的部分
        size_t tail_size = src.size() - pos - tag.size();
        if (tail_size > 0) {
            std::memcpy(dst, src.data() + pos + tag.size(), tail_size);
        }
        return result;
    }
    auto build_attr(const std::optional<badge::Color> &color, std::optional<badge::BBox> pos) {
        std::string attr;
        if (color) {
            attr += "fill=\"";
            attr += color->to_str();
            attr += "\"";
        }
        if (pos) {
            attr += " x=\"";
            attr += std::to_string(pos->x);
            attr += "\" y=\"";
            attr += std::to_string(pos->y);
            attr += "\" width=\"";
            attr += std::to_string(pos->w);
            attr += "\" height=\"";
            attr += std::to_string(pos->h);
        }
        return attr;
    }
}// namespace
namespace badge {
    BuiltinIcon::BuiltinIcon(std::string_view title, Color color, std::string_view icon) : title(std::move(title)), color(std::move(color)), icon(std::move(icon)) {}
    BuiltinIcon::BuiltinIcon(std::string_view title, std::string color, const void *data, size_t size)
        : title(std::move(title)),
          color(std::move(color), false),
          icon(reinterpret_cast<const char *>(data), size) {}

    std::string BuiltinIcon::get_uri(const std::optional<Color> &color, std::optional<BBox> pos) const {
        size_t replace_pos = icon.find(BUILTIN_REPLACE_TAG);
        if (replace_pos != std::string_view::npos) {
            auto icon_ = replace_tag(icon, replace_pos, BUILTIN_REPLACE_TAG, build_attr(color, pos));
            return data_url_prefix + b64::b64encode(reinterpret_cast<const uint8_t *>(icon_.data()), icon_.size());
        } else {
            return data_url_prefix + b64::b64encode(reinterpret_cast<const uint8_t *>(icon.data()), icon.size());
        }
    }
    std::string BuiltinIcon::get_svg(const std::optional<Color> &color, std::optional<BBox> pos) const {
        size_t replace_pos = icon.find(BUILTIN_REPLACE_TAG);
        if (replace_pos != std::string_view::npos) {
            return replace_tag(icon, replace_pos, BUILTIN_REPLACE_TAG, build_attr(color, pos));
        } else {
            return std::string{icon};
        }
    }
    const std::vector<std::shared_ptr<BuiltinIcon>> &BuiltinIcons::icons() {
        return instance().icons_;
    }
    Color BuiltinIcon::get_color() const { return color; }
    BuiltinIcons &BuiltinIcons::instance() {
        static BuiltinIcons instance;
        return instance;
    }

    BuiltinIcons::BuiltinIcons() {
        using namespace _assets;

        const std::string_view index(
                reinterpret_cast<const char *>(get_resource_data(ResId::__BADGECPP_ICON_IDX)),
                _assets::get_resource_size(ResId::__BADGECPP_ICON_IDX));

        const std::string_view data(
                reinterpret_cast<const char *>(get_resource_data(ResId::__BADGECPP_ICON_BIN)),
                _assets::get_resource_size(ResId::__BADGECPP_ICON_BIN));

        size_t ls = 0;// line start
        size_t le = 0;// line end
        while ((le = index.find('\n', le)) != std::string_view::npos) {
            // f"{offset}\t{length}\t{hex}\t{title}\n"

            bool ok = true;
            auto s_o_l = index.find('\t', ls);// split offset and length
            if (s_o_l == std::string_view::npos) ok = false;
            auto s_l_h = ok ? index.find('\t', s_o_l + 1) : std::string_view::npos;// split length and hex
            if (s_l_h == std::string_view::npos) ok = false;
            auto s_h_t = ok ? index.find('\t', s_l_h + 1) : std::string_view::npos;// split hex and title
            if (s_h_t == std::string_view::npos) ok = false;

            if (!ok || s_h_t >= le) throw std::runtime_error("[badgecpp::Icons] Invalid icon data");

            auto offset = std::stoul(std::string{index.substr(ls, s_o_l - ls)});
            auto length = std::stoul(std::string{index.substr(s_o_l + 1, s_l_h - s_o_l - 1)});
            std::string hex{index.substr(s_l_h + 1, s_h_t - s_l_h - 1)};
            std::string title{};

            icons_.emplace_back(std::make_shared<BuiltinIcon>(
                    index.substr(s_h_t + 1, le - s_h_t - 1),                      // title
                    '#' + std::string{index.substr(s_l_h + 1, s_h_t - s_l_h - 1)},// color
                    data.data() + offset, length                                  // size
                    ));

            ls = le = le + 1;
        }

        std::sort(icons_.begin(), icons_.end(),
                  [](const std::shared_ptr<BuiltinIcon> &a, const std::shared_ptr<BuiltinIcon> &b) -> bool {
                      return a->title < b->title;
                  });
    }
    std::shared_ptr<BuiltinIcon> BuiltinIcons::by_title(std::string_view title) {
        // 使用 std::lower_bound 进行二分查找
        const auto &icons = BuiltinIcons::icons();
        auto it = std::lower_bound(icons.begin(), icons.end(), title,
                                   [](const std::shared_ptr<BuiltinIcon> &icon, const std::string_view &value) -> bool {
                                       return icon->title < value;
                                   });

        if (it != icons.end() && (*it)->title == title) return *it;
        return nullptr;
    }
}// namespace badge

namespace {
    [[maybe_unused]] bool _ = ([]() {
        [[maybe_unused]] auto size = badge::BuiltinIcons::instance().icons().size();
        if (badge::version::is_debug())
            std::cout << "[debug] [badgecpp::BuiltinIcons] Loaded " << size << " icons" << std::endl;
        return true;
    })();
}