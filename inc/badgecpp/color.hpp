#ifndef BADGECPP_COLOR_HPP
#define BADGECPP_COLOR_HPP
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
namespace badge {
    struct Color final {
        static const std::unordered_map<std::string_view, Color> named_colors;

        std::array<uint8_t, 4> value{0, 0, 0, 255};
        static_assert(sizeof(value) == sizeof(uint32_t), "Color value must be 4 bytes");

        Color() = default;
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : value{r, g, b, a} {}
        Color(uint32_t rgba) : Color((rgba >> 24) & 0xff, (rgba >> 16) & 0xff, (rgba >> 8) & 0xff, rgba & 0xff) {}
        Color(std::string_view, bool fail_silent = true);


        /// @brief 转换为最短css颜色字符串
        /// @return #RGB or #RRGGBB or rgba(R,G,B,A)
        [[nodiscard]] std::string to_str() const;
        /// @return 颜色的亮度值, [0, 1]
        [[nodiscard]] double brightness() const;
        /// @brief 通过背景颜色获取文字颜色的Hex字符串
        /// @param bgColor 背景颜色
        /// @return [文字颜色, 阴影颜色]
        [[nodiscard]] std::pair<std::string, std::string> getColorHexPairForBackground();

        [[nodiscard]] uint8_t &r() { return value[0]; }
        [[nodiscard]] uint8_t &g() { return value[1]; }
        [[nodiscard]] uint8_t &b() { return value[2]; }
        [[nodiscard]] uint8_t &a() { return value[3]; }
        [[nodiscard]] uint32_t &rgba() { return *reinterpret_cast<uint32_t *>(value.data()); }
        [[nodiscard]] uint8_t r() const { return value[0]; }
        [[nodiscard]] uint8_t g() const { return value[1]; }
        [[nodiscard]] uint8_t b() const { return value[2]; }
        [[nodiscard]] uint8_t a() const { return value[3]; }
        [[nodiscard]] uint32_t rgba() const { return *reinterpret_cast<const uint32_t *>(value.data()); }

        bool operator==(const Color &other) const { return value == other.value; }
        bool operator!=(const Color &other) const { return value != other.value; }

        ///@brief 解析String
        ///@param v 结果输出. 解析失败时也可能改变部分值
        ///@return 是否解析成功
        static bool parseString(std::string_view str, std::array<uint8_t, 4> &v);
        ///@brief 解析Hex
        ///@param v 结果输出. 解析失败时也可能改变部分值
        ///@return 是否解析成功
        static bool parseHex(std::string_view hex, std::array<uint8_t, 4> &v);
        ///@brief 解析RGB
        ///@param v 结果输出. 解析失败时也可能改变部分值
        ///@return 是否解析成功
        static bool parseRGB(std::string_view rgb, std::array<uint8_t, 4> &v);
        ///@brief 解析RGBA
        ///@param v 结果输出. 解析失败时也可能改变部分值
        ///@return 是否解析成功
        static bool parseRGBA(std::string_view rgba, std::array<uint8_t, 4> &v);
        ///@brief 解析HSL
        ///@param v 结果输出. 解析失败时也可能改变部分值
        ///@return 是否解析成功
        static bool parseHSL(std::string_view hsl, std::array<uint8_t, 4> &v);
        ///@brief 解析HSLA
        ///@param v 结果输出. 解析失败时也可能改变部分值
        ///@return 是否解析成功
        static bool parseHSLA(std::string_view hsla, std::array<uint8_t, 4> &v);
        ///@brief 解析Name
        ///@param v 结果输出. 解析失败时也可能改变部分值
        ///@return 是否解析成功
        static bool parseName(std::string_view name, std::array<uint8_t, 4> &v);
        /// @brief 解析String, 解析失败返回nullopt
        static std::optional<Color> parseString(std::string_view str);
        /// @brief 解析Hex, 解析失败返回nullopt
        static std::optional<Color> parseHex(std::string_view hex);
        /// @brief 解析RGB, 解析失败返回nullopt
        static std::optional<Color> parseRGB(std::string_view rgb);
        /// @brief 解析RGBA, 解析失败返回nullopt
        static std::optional<Color> parseRGBA(std::string_view rgba);
        /// @brief 解析HSL, 解析失败返回nullopt
        static std::optional<Color> parseHSL(std::string_view hsl);
        /// @brief 解析HSLA, 解析失败返回nullopt
        static std::optional<Color> parseHSLA(std::string_view hsla);
        /// @brief 解析Name, 解析失败返回nullopt
        static std::optional<Color> parseName(std::string_view name);
    };
}// namespace badge
#endif