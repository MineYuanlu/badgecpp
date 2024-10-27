/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_BADGECPP__HPP_GUARD
#define BADGECPP_BADGECPP__HPP_GUARD
#include "badgecpp/xml.hpp"
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

    struct Badge final {
        std::optional<std::string> label_{};        ///< 标签文字
        std::optional<std::string> label_color_{};  ///< 标签背景颜色
        std::optional<std::string> message_{};      ///< 信息文字
        std::optional<std::string> message_color_{};///< 信息背景颜色
        Style style_ = FLAT;                        ///< 徽章样式
        std::optional<std::string> logo_{};         ///< 徽章logo(Base64)
        std::optional<uint> logo_width_{};          ///< 徽章logo宽度
        std::optional<std::string> id_suffix_{};    ///< 徽章id后缀


        [[nodiscard]] std::string makeBadge() const;
        [[nodiscard]] Xml makeBadgeXml() const;
    };
}// namespace badge
#endif// BADGECPP_BADGECPP__HPP_GUARD
