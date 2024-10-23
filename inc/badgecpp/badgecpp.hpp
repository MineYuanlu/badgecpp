/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_BADGECPP__HPP_GUARD
#define BADGECPP_BADGECPP__HPP_GUARD
#include <optional>
#include <sstream>
#include <string>
namespace badgecpp {

    static const constexpr uint DEFAULT_LOGO_HEIGHT = 14;
    std::string makeBadge(
            std::string_view label = "",
            std::string_view message = "",
            std::string_view color = "",
            std::string_view style = "",
            std::string_view logo = "",
            std::string_view suffix = "");

    enum Style {
        FLAT,
        FLAT_SQUARE,
        PLASTIC,
        FOR_THE_BADGE,
        SOCIAL,
    };

    struct Badge {
        std::optional<std::string> label_{};
        std::optional<std::string> label_color_{};
        std::optional<std::string> message_{};
        std::optional<std::string> message_color_{};
        Style style_ = FLAT;
        std::optional<std::string> logo_{};
        std::optional<std::string> id_suffix_{};

        std::optional<uint> logo_width_ = 0;
    };
}// namespace badgecpp
#endif// BADGECPP_BADGECPP__HPP_GUARD
