#include "badgecpp/icons.hpp"
#include "badgecpp/b64.hpp"
#include "badgecpp/resources.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string_view>
namespace {
    const std::string data_url_prefix = "data:image/svg+xml;base64,";
}
namespace badge {
    Icon::Icon(std::string title, Color color, std::string_view icon) : title(std::move(title)), color(std::move(color)), icon(std::move(icon)) {}
    Icon::Icon(std::string title, std::string color, const void *data, size_t size)
        : title(std::move(title)),
          color(std::move(color), false),
          icon(reinterpret_cast<const char *>(data), size) {}
    std::string Icon::get_uri() const {
        return data_url_prefix + b64::b64encode(reinterpret_cast<const uint8_t *>(icon.data()), icon.size());
    }
    const std::vector<Icon> &Icons::icons() {
        return instance().icons_;
    }
    Icons &Icons::instance() {
        static Icons instance;
        return instance;
    }

    Icons::Icons() {
        using namespace _assets;

        const std::string_view index(
                reinterpret_cast<const char *>(get_resource_data(ResId::__ICON_IDX)),
                _assets::get_resource_size(ResId::__ICON_IDX));

        const std::string_view data(
                reinterpret_cast<const char *>(get_resource_data(ResId::__ICON_BIN)),
                _assets::get_resource_size(ResId::__ICON_BIN));

        size_t ls = 0;// line start
        size_t le = 0;// line end
        while ((le = index.find('\n', le)) != std::string_view::npos) {
            // f"{offset}\t{length}\t{hex}\t{title}\n"

            auto s_o_l = index.find('\t', ls);
            auto s_l_h = index.find('\t', s_o_l + 1);
            auto s_h_t = index.find('\t', s_l_h + 1);

            if (s_h_t >= le) throw std::runtime_error("[badgecpp::Icons] Invalid icon data");

            auto offset = std::stoul(std::string{index.substr(ls, s_o_l - ls)});
            auto length = std::stoul(std::string{index.substr(s_o_l + 1, s_l_h - s_o_l - 1)});
            std::string hex{index.substr(s_l_h + 1, s_h_t - s_l_h - 1)};
            std::string title{index.substr(s_h_t + 1, le - s_h_t - 1)};

            icons_.emplace_back(std::move(title), '#' + std::move(hex), data.data() + offset, length);

            ls = le = le + 1;
        }
    }
}// namespace badge

namespace {
    [[maybe_unused]] bool _ = ([]() {
        std::cout << "Loaded " << badge::Icons::instance().icons().size() << " icons" << std::endl;
        return true;
    })();
}