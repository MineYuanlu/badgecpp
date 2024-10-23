#include "badgecpp/font.hpp"
#include "badgecpp/version.hpp"
#include <algorithm>
#include <codecvt>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <string_view>
namespace badge {

    std::u32string Font::toU32String(const std::string &str) {
        static thread_local std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        return converter.from_bytes(str);
    }

    Font::Font(std::vector<std::tuple<char32_t, char32_t, double>> widths, unsigned int size) : widths_(std::move(widths)), size_(size) {
        emWidth_ = widthOfCharCode(GUESS_CHAR, false);
    }


    Font Font::createByJsonFile(const std::string &filepath, unsigned int size) {
        std::ifstream file(filepath);
        if (!file.good()) throw std::runtime_error("[badgecpp::Front] Failed to open file: " + filepath);
        try {
            return Font::createByJsonStream(file, size);
        } catch (std::runtime_error &e) {
            std::string_view errmsg = e.what();
            static constexpr const std::string_view TAG = "[badgecpp::Front]";
            if (errmsg.compare(0, TAG.size(), TAG) == 0) throw std::runtime_error(e.what() + std::string{", file: "} + filepath);
            throw;
        }
    }
    Font Font::createByJsonString(const std::string &jsonString, unsigned int size) {
        std::istringstream iss(jsonString);
        return Font::createByJsonStream(iss, size);
    }
    Font Font::createByJsonStream(std::istream &file, unsigned int size) {
        constexpr const auto assert = [](bool cond, std::string msg = "") {
            if (!cond) throw std::runtime_error("[badgecpp::Front] Failed to read font: " + msg);
        };
        constexpr const auto MSG = [](std::string need, char got) {
            need = "need " + need;
            need += ", got: ";
            if (got == '\0') need += "EOF";
            need += got;
            return need;
        };

        /// 读取一个 [[int,int,double], ...] 的 json 数组

        std::string buffer(1024, 0);
        size_t buffer_size = buffer.length();
        size_t buffer_cursor = buffer_size;//初始为空


        /// @brief 读取一个字符, 带有buffer
        const auto read_char = [&]() -> char {
            if (buffer_size == 0) return '\0';
            if (buffer_cursor >= buffer_size) {
                file.read(&buffer[0], buffer.size());
                buffer_size = file.gcount();
                buffer_cursor = 0;
                if (buffer_size == 0) return '\0';
            }
            return buffer[buffer_cursor++];
        };
        /// @brief 读取一个字符, 使用read_char, 跳过空白字符
        const auto next_char = [&]() -> char {
            char c = read_char();
            while (c == ' ' || c == '\t' || c == '\r' || c == '\n') c = read_char();
            return c;
        };
        /// @brief 读取一个整数, 要求以","结尾
        const auto read_int = [&]() -> int32_t {
            int32_t x = 0, sign = 1;
            char c = next_char();
            if (c == '-') {
                sign = -1;
                c = next_char();
            }
            assert('0' <= c && c <= '9', MSG("0-9", c));
            do {
                x = (x << 1) + (x << 3) + (c - '0');// x * 10 + digit
                c = next_char();
            } while ('0' <= c && c <= '9');
            assert(c == ',', MSG("','", c));
            return x * sign;// 返回读取的整数
        };
        /// @brief 读取一个double, 要求以"]"结尾
        const auto read_double = [&]() -> double {
            double x = 0.0, factor = 1.0;
            char c = next_char();
            if (c == '-') {
                factor = -1.0;// 处理负号
                c = next_char();
            }
            assert(('0' <= c && c <= '9') || c == '.', MSG("0-9 or '.'", c));
            while ('0' <= c && c <= '9') {
                x = (x * 10.0) + (c - '0');
                c = next_char();
            }
            if (c == '.') {
                double place = 1.0;
                c = next_char();
                assert('0' <= c && c <= '9', MSG("0-9", c));
                do {
                    x += (c - '0') * (place /= 10.0);
                    c = next_char();
                } while ('0' <= c && c <= '9');
            }

            assert(c == ']', MSG("']'", c));

            return x * factor;
        };

        Font font;
        char next;
        next = next_char(), assert(next == '[', MSG("'['", next));
        while (true) {
            next = next_char(), assert(next == '[', MSG("'['", next));
            auto low = read_int();
            auto high = read_int();
            auto value = read_double();


            font.widths_.emplace_back(low, high, value);


            next = next_char();
            if (next == ']') break;
            assert(next == ',', MSG("','", next));
        }
        font.emWidth_ = font.widthOfCharCode(GUESS_CHAR, false);
        assert(font.emWidth_ > 0, std::string{"emWidth_ must be positive: '"} + GUESS_CHAR + "'(" + std::to_string(GUESS_CHAR) + "), got:" + std::to_string(font.emWidth_));
        return font;
    }


    double Font::widthOfCharCode(char32_t charCode, bool guess) const noexcept {
        if (isControlChar(charCode)) return 0.0;

        auto it = std::lower_bound(widths_.begin(), widths_.end(), charCode,
                                   [](const Range &range, char32_t value) {
                                       return std::get<1>(range) < value;
                                   });

        if (it != widths_.end() && std::get<0>(*it) <= charCode && charCode <= std::get<1>(*it)) {
            return std::get<2>(*it);
        }
        if (guess) return emWidth_;
        return -1;
    }

    double Font::widthOfString(const std::u32string &str, bool guess) const noexcept {
        double width = 0.0;
        for (auto c: str) {
            auto w = widthOfCharCode(c, guess);
            if (w < 0) return -1;
            width += w;
        }
        return width;
    }
    double Font::widthOfString(const std::string &s, bool guess) const noexcept {
        return widthOfString(toU32String(s), guess);
    }
    double Font::widthOfString(const std::optional<std::string> &s, bool guess) const noexcept { return s ? widthOfString(*s, guess) : 0; }
    double Font::widthOfString(const std::optional<std::u32string> &s, bool guess) const noexcept { return s ? widthOfString(*s, guess) : 0; }
    unsigned int Font::size() const noexcept { return size_; }
    double Font::emWidth() const noexcept { return emWidth_; }

    const Font *Fonts::get(const std::string &fontName) {
        auto itr = instance().fontMap.find(fontName);
        if (itr == instance().fontMap.end()) throw std::runtime_error("[badgecpp::Fonts::get] Not found font: " + fontName);
        return &(itr->second);
    }
    Fonts &Fonts::instance() {
        static Fonts fonts;
        return fonts;
    }
    const Font *Fonts::getDefault() { return get("verdana-11px-normal"); }

    const std::unordered_map<std::string, Font> &Fonts::getFonts() noexcept { return instance().fontMap; }

    void Fonts::createFont(std::string fontName, Font font) {
        if (fontName.empty()) throw std::runtime_error("[badgecpp::Fonts::createFont] Empty font name");

        if (!instance().fontMap.try_emplace(fontName, std::move(font)).second)
            throw std::runtime_error("[badgecpp::Fonts::createFont] Font already exists: " + fontName);

        if (version::is_debug()) {
            std::cout << "[badgecpp::Fonts::createFont] Font created: " << fontName << std::endl;
        }
    }

}// namespace badge

#include "badgecpp/resources.h"
namespace {
    using namespace badge::_assets;
    [[maybe_unused]] bool _ = ([]() {
        static constexpr std::string_view prefix = "./fonts/";
        static constexpr std::string_view suffix = ".json";
        for (const auto &[filepath, id]: get_resource_list()) {
            if (filepath.compare(0, prefix.size(), prefix) != 0 ||
                filepath.compare(filepath.size() - suffix.size(), suffix.size(), suffix) != 0) continue;

            std::string fontName = filepath.substr(prefix.size(), filepath.size() - prefix.size() - suffix.size());// exampe: "verdana-10px-bold"


            // 解析字体大小
            static constexpr char splitter = '-';
            static constexpr std::string_view size_str = "px";

            auto sizeStart = fontName.find(splitter);
            if (sizeStart == std::string::npos) throw std::runtime_error("[badgecpp::Fonts::createFont] Invalid font name: " + fontName);
            auto sizeUnit = fontName.find(size_str, sizeStart + 1);
            if (sizeUnit == std::string::npos) throw std::runtime_error("[badgecpp::Fonts::createFont] Invalid font name: " + fontName);
            auto sizeEnd = fontName.find(splitter, sizeUnit + size_str.size());
            if (sizeEnd == std::string::npos) throw std::runtime_error("[badgecpp::Fonts::createFont] Invalid font name: " + fontName);

            std::string sizeStr = fontName.substr(sizeStart + 1, sizeEnd - sizeStart - 1);
            unsigned int size = 0;
            try {
                size = std::stoul(sizeStr);
            } catch (const std::exception &e) {
                throw std::runtime_error("[badgecpp::Fonts::createFont] Invalid font size: " + sizeStr + ", " + e.what() + ", file: " + filepath);
            }

            // 读取数据
            const uint8_t *data = get_resource_data(ResId::__FONTS_HELVETICA_11PX_BOLD_JSON);
            uint32_t sz = get_resource_size(ResId::__FONTS_HELVETICA_11PX_BOLD_JSON);

            const std::string jsonString{reinterpret_cast<const char *>(data), sz};

            // 构造字体
            badge::Fonts::createFont(fontName, badge::Font::createByJsonString(jsonString, size));
        }

        return true;
    })();
}// namespace