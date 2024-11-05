/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_BADGE_HPP_GUARD
#define BADGECPP_BADGE_HPP_GUARD
#include "badgecpp/color.hpp"
#include "badgecpp/icons.hpp"
#include "badgecpp/xml.hpp"
#include <memory>
#include <optional>
#include <string>
namespace badge {


    enum Style {
        FLAT,
        FLAT_SQUARE,
        PLASTIC,
        FOR_THE_BADGE,
        SOCIAL,
    };
    constexpr const char *style_str(Style style) {
        switch (style) {
                // clang-format off
            case FLAT: return "FLAT";
            case FLAT_SQUARE: return "FLAT_SQUARE";
            case PLASTIC: return "PLASTIC";
            case FOR_THE_BADGE: return "FOR_THE_BADGE";
            case SOCIAL: return "SOCIAL";
                // clang-format on
        }
        return "UNKNOWN";
    }

    struct Badge final {
        std::optional<std::string> label_{};      ///< 标签文字
        std::optional<Color> label_color_{};      ///< 标签背景颜色
        std::optional<std::string> message_{};    ///< 信息文字
        std::optional<Color> message_color_{};    ///< 信息背景颜色
        Style style_ = FLAT;                      ///< 徽章样式
        std::shared_ptr<const Icon> logo_{};      ///< 徽章logo
        std::optional<Color> logo_color_{};       ///< 徽章logo颜色, 默认为logo自带的颜色
        std::optional<unsigned int> logo_width_{};///< 徽章logo宽度, 默认为14
        std::optional<std::string> id_suffix_{};  ///< 徽章id后缀, 用于区分不同badge


        [[nodiscard]] std::string makeBadge() const;
        [[nodiscard]] Xml makeBadgeXml() const;
    };
}// namespace badge
#endif// BADGECPP_BADGE_HPP_GUARD
