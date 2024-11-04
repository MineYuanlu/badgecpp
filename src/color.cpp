#include "badgecpp/color.hpp"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

namespace {
    bool startsWith(const std::string_view &str, const std::string_view &prefix) {
        return str.size() > prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
    }
    std::string_view trim(const std::string_view &str) {
        auto first = str.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string_view::npos)
            return "";
        auto last = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(first, last - first + 1);
    }


    /// @brief 解析0-255的整数
    /// @return 0-255
    uint8_t fromI(const std::string_view &num, int base = 10) {
        int value = 0;
        size_t pos = 0;
        try {
            value = std::stoi(std::string(num), &pos, base);
        } catch (...) { throw std::runtime_error(""); }
        if (pos != num.size() || value < 0 || value > 255) throw std::runtime_error("");
        return static_cast<uint8_t>(value);
    }
    /// @brief 解析0-1的浮点数
    /// @return 0-255
    uint8_t fromD(const std::string_view &num) {
        float value = 0.0f;
        size_t pos = 0;
        try {
            value = std::stof(std::string(num), &pos);
        } catch (...) { throw std::runtime_error(""); }
        if (pos != num.size() || value < 0.0f || value > 1.0f) throw std::runtime_error("");
        return static_cast<uint8_t>(std::round(value * 255));
    }
    /// @brief 解析0-360的角度
    /// @return 0-360
    int fromH(const std::string_view &num) {
        int value = 0;
        size_t pos = 0;
        try {
            value = std::stoi(std::string(num), &pos);
        } catch (...) { throw std::runtime_error(""); }
        if (pos != num.size() || value < 0 || value > 360) throw std::runtime_error("");
        return value;
    }
    /// @brief 解析0%-100%百分比
    /// @return 0-1
    float fromP(const std::string_view &num) {
        if (num.back() != '%') throw std::runtime_error("");
        float value = 0.0f;
        size_t pos = 0;
        try {
            value = std::stof(std::string(num.substr(0, num.size() - 1)), &pos);
        } catch (...) { throw std::runtime_error(""); }
        if (pos != num.size() - 1 || value < 0.0f || value > 100.0f) throw std::runtime_error("");
        return value / 100.0f;
    }
    /// @brief 辅助函数: 将start设为end下一个字符, 然后从str的start位置开始, 找到need下一次出现的位置, 设置end位置
    /// @return true表示解析失败, false表示解析成功
    constexpr auto next = [](const std::string_view &str, char need, size_t &start, size_t &end) {
        start = end + 1;
        end = str.find(need, start);
        return end == std::string_view::npos;
    };

    std::array<uint8_t, 4> hsl2rgb(int H, float S, float L, uint8_t A) {
        float C = (1 - std::fabs(2 * L - 1)) * S;            // Chroma
        float X = C * (1 - std::fabs(fmod(H / 60.0, 2) - 1));// Intermediate value
        float m = L - C / 2;                                 // Match lightness

        float rp, gp, bp;
        switch ((H % 360) / 60) {
                // clang-format off
            case 0: rp = C; gp = X; bp = 0; break;
            case 1: rp = X; gp = C; bp = 0; break;
            case 2: rp = 0; gp = C; bp = X; break;
            case 3: rp = 0; gp = X; bp = C; break;
            case 4: rp = X; gp = 0; bp = C; break;
            case 5: rp = C; gp = 0; bp = X; break;
                // clang-format on
        }

        return {
                uint8_t((rp + m) * 255),
                uint8_t((gp + m) * 255),
                uint8_t((bp + m) * 255),
                255,
        };
    }

}// namespace
namespace badge {
    std::size_t ColorHash::operator()(const badge::Color &c) const {
        return c.rgba();
    }
    Color::Color(std::string_view input, bool fail_silent) {
        decltype(value) tmp = value;
        if (parseString(input, tmp)) value = tmp;
        else if (!fail_silent)
            throw std::invalid_argument("[badgecpp::Color] Invalid color string: " + std::string(input));
    }


    bool Color::parseString(std::string_view input, std::array<uint8_t, 4> &tmp) {
        auto str = trim(input);
        if (str.empty()) return false;
        if (parseHex(str, tmp)) return true;
        if (str.back() == ')') {
            if (parseRGB(str, tmp)) return true;
            if (parseRGBA(str, tmp)) return true;
            if (parseHSL(str, tmp)) return true;
            if (parseHSLA(str, tmp)) return true;
        }
        if (parseName(str, tmp)) return true;
        return false;
    }
    bool Color::parseHex(std::string_view str, std::array<uint8_t, 4> &v) {
        if (str[0] != '#') return false;
        if (str.size() == 7) {
            try {
                for (size_t i = 0; i < 3; ++i) v[i] = fromI(str.substr(i * 2 + 1, 2), 16);

            } catch (...) { return false; }
            return true;
        }
        if (str.size() == 4) {
            try {
                for (size_t i = 0; i < 3; ++i) v[i] = fromI(str.substr(i + 1, 2), 16) * 17;
            } catch (...) { return false; }
            return true;
        }
        return false;
    }
    bool Color::parseRGB(std::string_view str, std::array<uint8_t, 4> &v) {
        if (!startsWith(str, "rgb(") || str.back() != ')') return false;
        size_t start, end = 3;
        try {
            for (int i = 0; i < 3; i++) {
                if (next(str, i == 2 ? ')' : ',', start, end)) return false;
                v[i] = fromI(trim(str.substr(start, end - start)));
            }
        } catch (...) { return false; }
        return true;
    }
    bool Color::parseRGBA(std::string_view str, std::array<uint8_t, 4> &v) {
        if (!startsWith(str, "rgba(") || str.back() != ')') return false;
        size_t start, end = 4;
        try {
            for (int i = 0; i < 3; i++) {
                if (next(str, ',', start, end)) return false;
                v[i] = fromI(trim(str.substr(start, end - start)));
            }
            if (next(str, ')', start, end)) return false;
            v[3] = fromD(trim(str.substr(start, end - start)));
        } catch (...) { return false; }
        return true;
    }
    bool Color::parseHSL(std::string_view str, std::array<uint8_t, 4> &v) {
        if (!startsWith(str, "hsl(") || str.back() != ')') return false;
        size_t start, end = 3;
        try {
            if (next(str, ',', start, end)) return false;
            const auto H = fromH(trim(str.substr(start, end - start)));
            if (next(str, ',', start, end)) return false;
            const auto S = fromP(trim(str.substr(start, end - start)));
            if (next(str, ')', start, end)) return false;
            const auto L = fromP(trim(str.substr(start, end - start)));
            v = hsl2rgb(H, S, L, 255);
        } catch (...) { return false; }
        return true;
    }
    bool Color::parseHSLA(std::string_view str, std::array<uint8_t, 4> &v) {
        if (!startsWith(str, "hsla(") || str.back() != ')') return false;
        size_t start, end = 3;
        try {
            if (next(str, ',', start, end)) return false;
            const auto H = fromH(trim(str.substr(start, end - start)));
            if (next(str, ',', start, end)) return false;
            const auto S = fromP(trim(str.substr(start, end - start)));
            if (next(str, ',', start, end)) return false;
            const auto L = fromP(trim(str.substr(start, end - start)));
            if (next(str, ')', start, end)) return false;
            const auto A = fromI(trim(str.substr(start, end - start)));
            v = hsl2rgb(H, S, L, A);
        } catch (...) { return false; }
        return true;
    }
    bool Color::parseName(std::string_view str, std::array<uint8_t, 4> &v) {
        auto itr = named_colors.find(str);
        if (itr == named_colors.end()) return false;
        v = itr->second.value;
        return true;
    }


    std::optional<Color> Color::parseString(std::string_view str) {
        Color tmp;
        return parseString(str, tmp.value) ? std::make_optional(std::move(tmp)) : std::nullopt;
    }
    std::optional<Color> Color::parseHex(std::string_view str) {
        Color tmp;
        return parseHex(str, tmp.value) ? std::make_optional(std::move(tmp)) : std::nullopt;
    }
    std::optional<Color> Color::parseRGB(std::string_view str) {
        Color tmp;
        return parseRGB(str, tmp.value) ? std::make_optional(std::move(tmp)) : std::nullopt;
    }
    std::optional<Color> Color::parseRGBA(std::string_view str) {
        Color tmp;
        return parseRGBA(str, tmp.value) ? std::make_optional(std::move(tmp)) : std::nullopt;
    }
    std::optional<Color> Color::parseHSL(std::string_view str) {
        Color tmp;
        return parseHSL(str, tmp.value) ? std::make_optional(std::move(tmp)) : std::nullopt;
    }
    std::optional<Color> Color::parseHSLA(std::string_view str) {
        Color tmp;
        return parseHSLA(str, tmp.value) ? std::make_optional(std::move(tmp)) : std::nullopt;
    }
    std::optional<Color> Color::parseName(std::string_view str) {
        Color tmp;
        return parseName(str, tmp.value) ? std::make_optional(std::move(tmp)) : std::nullopt;
    }


    std::string Color::to_str() const {
        if (a() == 255) {
            auto name = named_colors_reverse.find(*this);// 颜色名称
            static constexpr const char *HEX = "0123456789ABCDEF";
            if ((r() >> 4 == (r() & 0xF)) &&
                (g() >> 4 == (g() & 0xF)) &&
                (b() >> 4 == (b() & 0xF))) {
                if (name != named_colors_reverse.end() && name->second.length() < 4) return name->second;
                return {'#', HEX[r() & 0xF], HEX[g() & 0xF], HEX[b() & 0xF]};
            } else {
                if (name != named_colors_reverse.end() && name->second.length() < 7) return name->second;
                return {
                        '#',
                        HEX[r() >> 4],
                        HEX[r() & 0xF],
                        HEX[g() >> 4],
                        HEX[g() & 0xF],
                        HEX[b() >> 4],
                        HEX[b() & 0xF],
                };
            }
        } else {
            std::string str = "rgba(";
            str += std::to_string(r());
            str += ',';
            str += std::to_string(g());
            str += ',';
            str += std::to_string(b());
            str += ',';
            auto A = std::to_string(a() / 255.0);
            if (auto dot = A.find('.'); dot)
                str += A.substr(dot, 1 + 4);
            else
                str += A;
            str += ')';
            return str;
        }
    }

    std::optional<std::string> Color::name() const {
        auto name = named_colors_reverse.find(*this);
        return name != named_colors_reverse.end() ? std::make_optional(name->second) : std::nullopt;
    }


    double Color::brightness() const {
        static constexpr const double factor = 1.0 / 255000;
        return factor * (value[0] * 299 + value[1] * 587 + value[2] * 114);
    }

    std::pair<std::string, std::string> Color::getColorHexPairForBackground() {
        static constexpr const double threshold = 0.69;
        if (brightness() <= threshold) {
            return {"#fff", "#010101"};
        } else {
            return {"#333", "#ccc"};
        }
    }


    const std::unordered_map<std::string_view, Color> Color::named_colors = {
            {"aliceblue", {240, 248, 255}},
            {"antiquewhite", {250, 235, 215}},
            {"aqua", {0, 255, 255}},
            {"aquamarine", {127, 255, 212}},
            {"azure", {240, 255, 255}},
            {"beige", {245, 245, 220}},
            {"bisque", {255, 228, 196}},
            {"black", {0, 0, 0}},
            {"blanchedalmond", {255, 235, 205}},
            {"blue", {0, 0, 255}},
            {"blueviolet", {138, 43, 226}},
            {"brown", {165, 42, 42}},
            {"burlywood", {222, 184, 135}},
            {"cadetblue", {95, 158, 160}},
            {"chartreuse", {127, 255, 0}},
            {"chocolate", {210, 105, 30}},
            {"coral", {255, 127, 80}},
            {"cornflowerblue", {100, 149, 237}},
            {"cornsilk", {255, 248, 220}},
            {"crimson", {220, 20, 60}},
            {"cyan", {0, 255, 255}},
            {"darkblue", {0, 0, 139}},
            {"darkcyan", {0, 139, 139}},
            {"darkgoldenrod", {184, 134, 11}},
            {"darkgray", {169, 169, 169}},
            {"darkgreen", {0, 100, 0}},
            {"darkgrey", {169, 169, 169}},
            {"darkkhaki", {189, 183, 107}},
            {"darkmagenta", {139, 0, 139}},
            {"darkolivegreen", {85, 107, 47}},
            {"darkorange", {255, 140, 0}},
            {"darkorchid", {153, 50, 204}},
            {"darkred", {139, 0, 0}},
            {"darksalmon", {233, 150, 122}},
            {"darkseagreen", {143, 188, 143}},
            {"darkslateblue", {72, 61, 139}},
            {"darkslategray", {47, 79, 79}},
            {"darkslategrey", {47, 79, 79}},
            {"darkturquoise", {0, 206, 209}},
            {"darkviolet", {148, 0, 211}},
            {"deeppink", {255, 20, 147}},
            {"deepskyblue", {0, 191, 255}},
            {"dimgray", {105, 105, 105}},
            {"dimgrey", {105, 105, 105}},
            {"dodgerblue", {30, 144, 255}},
            {"firebrick", {178, 34, 34}},
            {"floralwhite", {255, 250, 240}},
            {"forestgreen", {34, 139, 34}},
            {"fuchsia", {255, 0, 255}},
            {"gainsboro", {220, 220, 220}},
            {"ghostwhite", {248, 248, 255}},
            {"gold", {255, 215, 0}},
            {"goldenrod", {218, 165, 32}},
            {"gray", {128, 128, 128}},
            {"green", {0, 128, 0}},
            {"greenyellow", {173, 255, 47}},
            {"grey", {128, 128, 128}},
            {"honeydew", {240, 255, 240}},
            {"hotpink", {255, 105, 180}},
            {"indianred", {205, 92, 92}},
            {"indigo", {75, 0, 130}},
            {"ivory", {255, 255, 240}},
            {"khaki", {240, 230, 140}},
            {"lavender", {230, 230, 250}},
            {"lavenderblush", {255, 240, 245}},
            {"lawngreen", {124, 252, 0}},
            {"lemonchiffon", {255, 250, 205}},
            {"lightblue", {173, 216, 230}},
            {"lightcoral", {240, 128, 128}},
            {"lightcyan", {224, 255, 255}},
            {"lightgoldenrodyellow", {250, 250, 210}},
            {"lightgray", {211, 211, 211}},
            {"lightgreen", {144, 238, 144}},
            {"lightgrey", {211, 211, 211}},
            {"lightpink", {255, 182, 193}},
            {"lightsalmon", {255, 160, 122}},
            {"lightseagreen", {32, 178, 170}},
            {"lightskyblue", {135, 206, 250}},
            {"lightslategray", {119, 136, 153}},
            {"lightslategrey", {119, 136, 153}},
            {"lightsteelblue", {176, 196, 222}},
            {"lightyellow", {255, 255, 224}},
            {"lime", {0, 255, 0}},
            {"limegreen", {50, 205, 50}},
            {"linen", {250, 240, 230}},
            {"magenta", {255, 0, 255}},
            {"maroon", {128, 0, 0}},
            {"mediumaquamarine", {102, 205, 170}},
            {"mediumblue", {0, 0, 205}},
            {"mediumorchid", {186, 85, 211}},
            {"mediumpurple", {147, 112, 219}},
            {"mediumseagreen", {60, 179, 113}},
            {"mediumslateblue", {123, 104, 238}},
            {"mediumspringgreen", {0, 250, 154}},
            {"mediumturquoise", {72, 209, 204}},
            {"mediumvioletred", {199, 21, 133}},
            {"midnightblue", {25, 25, 112}},
            {"mintcream", {245, 255, 250}},
            {"mistyrose", {255, 228, 225}},
            {"moccasin", {255, 228, 181}},
            {"navajowhite", {255, 222, 173}},
            {"navy", {0, 0, 128}},
            {"oldlace", {253, 245, 230}},
            {"olive", {128, 128, 0}},
            {"olivedrab", {107, 142, 35}},
            {"orange", {255, 165, 0}},
            {"orangered", {255, 69, 0}},
            {"orchid", {218, 112, 214}},
            {"palegoldenrod", {238, 232, 170}},
            {"palegreen", {152, 251, 152}},
            {"paleturquoise", {175, 238, 238}},
            {"palevioletred", {219, 112, 147}},
            {"papayawhip", {255, 239, 213}},
            {"peachpuff", {255, 218, 185}},
            {"peru", {205, 133, 63}},
            {"pink", {255, 192, 203}},
            {"plum", {221, 160, 221}},
            {"powderblue", {176, 224, 230}},
            {"purple", {128, 0, 128}},
            {"rebeccapurple", {102, 51, 153}},
            {"red", {255, 0, 0}},
            {"rosybrown", {188, 143, 143}},
            {"royalblue", {65, 105, 225}},
            {"saddlebrown", {139, 69, 19}},
            {"salmon", {250, 128, 114}},
            {"sandybrown", {244, 164, 96}},
            {"seagreen", {46, 139, 87}},
            {"seashell", {255, 245, 238}},
            {"sienna", {160, 82, 45}},
            {"silver", {192, 192, 192}},
            {"skyblue", {135, 206, 235}},
            {"slateblue", {106, 90, 205}},
            {"slategray", {112, 128, 144}},
            {"slategrey", {112, 128, 144}},
            {"snow", {255, 250, 250}},
            {"springgreen", {0, 255, 127}},
            {"steelblue", {70, 130, 180}},
            {"tan", {210, 180, 140}},
            {"teal", {0, 128, 128}},
            {"thistle", {216, 191, 216}},
            {"tomato", {255, 99, 71}},
            {"turquoise", {64, 224, 208}},
            {"violet", {238, 130, 238}},
            {"wheat", {245, 222, 179}},
            {"white", {255, 255, 255}},
            {"whitesmoke", {245, 245, 245}},
            {"yellow", {255, 255, 0}},
            {"yellowgreen", {154, 205, 50}},
    };
    const std::unordered_map<Color, std::string, ColorHash> Color::named_colors_reverse = []() {
        std::unordered_map<Color, std::string, ColorHash> ret;
        for (const auto &[name, color]: named_colors) {
            ret.try_emplace(color, name);
        }
        return ret;
    }();
}// namespace badge