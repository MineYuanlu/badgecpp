#ifndef BADGECPP_ICONS_HPP
#define BADGECPP_ICONS_HPP
#include "color.hpp"
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
namespace badge {
    struct Icon {
        virtual ~Icon() = default;
        /// @brief 获取图标uri格式的数据
        [[nodiscard]] virtual std::string get_uri() const = 0;
        /// @brief 获取图标基准颜色
        [[nodiscard]] virtual Color get_color() const = 0;
    };

    /// @see BuiltinIcons
    struct BuiltinIcon final : public Icon {
        std::string_view title;
        Color color;
        std::string_view icon;

        BuiltinIcon(std::string_view title, Color color, std::string_view icon);
        BuiltinIcon(std::string_view title, std::string color, const void *data, size_t size);

        [[nodiscard]] std::string get_uri() const override;
        [[nodiscard]] Color get_color() const override;
    };

    /// @brief From Simple Icons
    /// https://github.com/simple-icons
    struct BuiltinIcons final {

        /// @brief 获取所有的内置图标
        static const std::vector<BuiltinIcon> &icons();

        /// @brief 获取内置图标管理器实例
        static BuiltinIcons &instance();

        /// @brief 根据标题查找内置图标
        /// @param title 图标标题
        /// @return 内置图标实例，如果没有找到则返回 nullptr
        static const BuiltinIcon *by_title(std::string_view title);

    private:
        BuiltinIcons();
        std::vector<BuiltinIcon> icons_;
    };
}// namespace badge
#endif// BADGECPP_ICONS_HPP