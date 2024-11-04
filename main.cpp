#include "badgecpp/badge.hpp"
#include "badgecpp/icons.hpp"
#include "badgecpp/xml.hpp"
#include <fstream>

static const auto LOGO = badge::BuiltinIcons::by_title("C++")->get_uri();
badge::Xml makeSome(badge::Style style) {
    static int count = 0;
    using namespace badge;
    Badge svg{
            "label",
            std::nullopt,
            "message",
            std::nullopt,
            style,
            std::nullopt,
            std::nullopt,
            "id",
    };
    svg.id_suffix_ = "id" + std::to_string(++count);
    auto label_msg = svg.makeBadgeXml();

    svg.id_suffix_ = "id" + std::to_string(++count);
    svg.label_ = std::nullopt;
    auto msg_only = svg.makeBadgeXml();

    svg.id_suffix_ = "id" + std::to_string(++count);
    svg.message_ = std::nullopt;
    svg.label_ = "label";
    auto label_only = svg.makeBadgeXml();

    svg.id_suffix_ = "id" + std::to_string(++count);
    svg.label_ = std::nullopt;
    auto none = svg.makeBadgeXml();

    svg.id_suffix_ = "id" + std::to_string(++count);
    svg.logo_ = LOGO;
    auto logo_only = svg.makeBadgeXml();

    svg.id_suffix_ = "id" + std::to_string(++count);
    svg.message_ = "message";
    auto logo_msg = svg.makeBadgeXml();

    svg.id_suffix_ = "id" + std::to_string(++count);
    svg.label_ = "label";
    auto logo_label_msg = svg.makeBadgeXml();

    return {
            "",
            Xml{"td", style_str(style)},
            Xml{"td", label_msg},
            Xml{"td", msg_only},
            Xml{"td", label_only},
            Xml{"td", none},
            Xml{"td", logo_only},
            Xml{"td", logo_msg},
            Xml{"td", logo_label_msg},
    };
}
int main() {
    using namespace badge;
    Xml html{
            "html",
            {{"lang", "en"}},
            Xml{
                    "body",
                    Xml{"h1", "badge:"},
                    Xml{
                            "table",
                            {
                                    {"border", "1"},
                            },
                            Xml{
                                    "tr",
                                    Xml{"th", "STYLE"},
                                    Xml{"th", "label_msg"},
                                    Xml{"th", "msg_only"},
                                    Xml{"th", "label_only"},
                                    Xml{"th", "none"},
                                    Xml{"th", "logo_only"},
                                    Xml{"th", "logo_msg"},
                                    Xml{"th", "logo_label_msg"},
                            },
                            Xml{"tr", makeSome(Style::FLAT)},
                            Xml{"tr", makeSome(Style::FLAT_SQUARE)},
                            Xml{"tr", makeSome(Style::PLASTIC)},
                            Xml{"tr", makeSome(Style::SOCIAL)},
                            Xml{"tr", makeSome(Style::FOR_THE_BADGE)},
                    }},
    };
    std::string file = "badge.html";
    std::ofstream fout(file);
    fout << html.render();
    return 0;
}