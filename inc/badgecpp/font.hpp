/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_FONT__HPP_GUARD
#define BADGECPP_FONT__HPP_GUARD
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
namespace badge {


    class Font final {
        using Range = std::tuple<char32_t, char32_t, double>;
        /// @brief [lower, upper, width]
        /// @details 其中 lower 和 upper 表示字符编码的范围, width 表示该范围内字符的宽度
        std::vector<Range> widths_{};
        uint size_ = -1;
        double emWidth_ = -2;

    public:
        static constexpr char GUESS_CHAR = 'm';


    public:
        static constexpr bool isControlChar(char32_t charCode) noexcept {
            return charCode <= 31 || charCode == 127;
        }


        /// @brief 构造函数
        Font(std::vector<std::tuple<char32_t, char32_t, double>> widths, uint size);

        /// @brief 获取char宽度
        /// @param c 字符编码
        /// @param guess 如果宽度未知, 是否猜测为 emWidth_
        /// @return 字符宽度, -1代表未知且不猜测
        [[nodiscard]] double widthOfCharCode(char32_t c, bool guess = true) const noexcept;

        /// @brief 获取字符串宽度
        /// @param s 字符串
        /// @param guess 如果宽度未知, 是否猜测为 emWidth_
        /// @return 字符串宽度, -1代表有任一未知且不猜测
        [[nodiscard]] double widthOfString(const std::u32string &s, bool guess = true) const noexcept;

        /// @brief 获取字符串宽度
        /// @param s 字符串
        /// @param guess 如果宽度未知, 是否猜测为 emWidth_
        /// @return 字符串宽度, -1代表有任一未知且不猜测
        [[nodiscard]] double widthOfString(const std::string &s, bool guess = true) const noexcept;


        /// @brief 获取字符串宽度
        /// @param s 字符串, 如果为nullopt则返回0
        /// @param guess 如果宽度未知, 是否猜测为 emWidth_
        /// @return 字符串宽度, -1代表有任一未知且不猜测
        [[nodiscard]] double widthOfString(const std::optional<std::string> &s, bool guess = true) const noexcept;

        /// @brief 获取字符串宽度
        /// @param s 字符串, 如果为nullopt则返回0
        /// @param guess 如果宽度未知, 是否猜测为 emWidth_
        /// @return 字符串宽度, -1代表有任一未知且不猜测
        [[nodiscard]] double widthOfString(const std::optional<std::u32string> &s, bool guess = true) const noexcept;

        /// @brief 获取字体像素大小
        [[nodiscard]] uint size() const noexcept;

        /// @brief 获取猜测宽度
        [[nodiscard]] double emWidth() const noexcept;

        /// @brief 从json文件构造
        /// @param filepath json文件路径
        /// @param size 像素大小
        /// @throws std::runtime_error 无法解析json文件
        static Font createByJsonFile(const std::string &filepath, uint size);
        /// @brief 从json字符串构造
        /// @param jsonString json字符串
        /// @param size 像素大小
        /// @throws std::runtime_error 无法解析json字符串
        static Font createByJsonString(const std::string &jsonString, uint size);
        /// @brief 从json流构造
        /// @param stream json流
        /// @param size 像素大小
        /// @throws std::runtime_error 无法解析json流
        static Font createByJsonStream(std::istream &stream, uint size);

    private:
        Font() = default;
    };

    // enum FontsEnum {
    //     HELVETICA_11_BOLD = 0011,
    //     VERDANA_10_NORMAL = 0100,
    //     VERDANA_10_BOLD = 0101,
    //     VERDANA_11_NORMAL = 0110,
    // };


    /// @brief 字体管理器
    /// @note 单例模式
    struct Fonts final {
        // static constexpr const std::tuple<FontsEnum, std::string_view, uint> FILES[] = {
        //         {HELVETICA_11_BOLD, "helvetica-11px-bold.json", 11},
        //         {VERDANA_10_NORMAL, "verdana-10px-normal.json", 10},
        //         {VERDANA_10_BOLD, "verdana-10px-bold.json", 10},
        //         {VERDANA_11_NORMAL, "verdana-11px-normal.json", 11},
        // };

        /// @brief 获取默认字体
        static const Font &getDefault();


        /// @brief 获取字体
        /// @param fontName 字体枚举值
        /// @throw std::runtime_error 字体不存在
        /// @return 字体
        static const Font &get(const std::string &fontName);

        /// @brief 获取所有字体
        static const std::unordered_map<std::string, Font> &getFonts() noexcept;

        /// @brief 创建字体
        static void createFont(std::string fontName, Font font);

    private:
        std::unordered_map<std::string, Font> fontMap;
        Fonts() = default;
        static Fonts &instance();
    };
#define BADGECPP_REGISTER_FONT(name,px,data) namespace{struct FontRegister{\
    FontRegister()\
    };}



}// namespace badge
#endif// BADGECPP_FONT__HPP_GUARD
