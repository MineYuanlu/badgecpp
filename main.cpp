#include "badgecpp/badge.hpp"
#include "badgecpp/xml.hpp"
#include <fstream>

static constexpr auto LOGO = "data:image/svg+xml;base64,PHN2ZyB0PSIxNzMwMDEwODg0NDc0IiBjbGFzcz0iaWNvbiIgdmlld0JveD0iMCAwIDEwMjQgMTAyNCIgdmVyc2lvbj0iMS4xIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHAtaWQ9IjEyMTc1IiB3aWR0aD0iMjAwIiBoZWlnaHQ9IjIwMCI+PHBhdGggZD0iTTY0NS44MDI2NjcgMzY2LjY3NzMzM2MtMjEuOTMwNjY3LTEzNS4xNjgtNzMuNzI4LTIzMC4xMDEzMzMtMTMzLjkzMDY2Ny0yMzAuMTAxMzMzcy0xMTIuMDQyNjY3IDk0LjkzMzMzMy0xMzMuOTczMzMzIDIzMC4xMDEzMzN6TTM2Ni41MDY2NjcgNTEyLjA0MjY2N2E5NDguODY0IDk0OC44NjQgMCAwIDAgNC45OTIgOTYuODUzMzMzaDI4MC41MzMzMzNhOTQ4LjgyMTMzMyA5NDguODIxMzMzIDAgMCAwIDQuOTA2NjY3LTk2Ljg1MzMzMyA5NDguODY0IDk0OC44NjQgMCAwIDAtNC45OTItOTYuODk2SDM3MS40OTg2NjdhOTQ4Ljg2NCA5NDguODY0IDAgMCAwLTQuOTkyIDk2Ljg5NnogbTQ5MS41Mi0xNDUuMzIyNjY3YTM3Ni4yMzQ2NjcgMzc2LjIzNDY2NyAwIDAgMC0yMzkuMTg5MzM0LTIxNC4zNTczMzMgNTEwLjkzMzMzMyA1MTAuOTMzMzMzIDAgMCAxIDc1LjY5MDY2NyAyMTQuMzU3MzMzek00MDQuNjUwNjY3IDE1Mi4zMmEzNzUuOTc4NjY3IDM3NS45Nzg2NjcgMCAwIDAtMjM4LjkzMzMzNCAyMTQuMzU3MzMzaDE2My40MTMzMzRhNTA4Ljg0MjY2NyA1MDguODQyNjY3IDAgMCAxIDc1LjUyLTIxNC4zNTczMzN6TTg3NC4yNCA0MTUuMTA0aC0xNzMuNjUzMzMzYzMuMiAzMS43ODY2NjcgNC45OTIgNjQuMzQxMzMzIDQuOTkyIDk2Ljg5NnMtMS44MzQ2NjcgNjUuMTA5MzMzLTQuOTkyIDk2Ljg5NmgxNzMuNTI1MzMzQTM3Mi4xODEzMzMgMzcyLjE4MTMzMyAwIDAgMCA4ODcuMTI1MzMzIDUxMmEzNzguNzA5MzMzIDM3OC43MDkzMzMgMCAwIDAtMTIuOC05Ni44OTZ6TTMxOC4wOCA1MTIuMDQyNjY3YzAtMzIuNTU0NjY3IDEuODM0NjY3LTY1LjEwOTMzMyA0Ljk5Mi05Ni44OTZoLTE3My42NTMzMzNhMzY2LjkzMzMzMyAzNjYuOTMzMzMzIDAgMCAwIDAgMTkzLjc0OTMzM2gxNzMuNDgyNjY2YTEwMjQuMzg0IDEwMjQuMzg0IDAgMCAxLTQuODIxMzMzLTk2Ljg1MzMzM3ogbTU5LjczMzMzMyAxNDUuMzIyNjY2YzIyLjAxNiAxMzUuMTY4IDczLjc3MDY2NyAyMzAuMTQ0IDEzNC4wNTg2NjcgMjMwLjE0NHMxMTIuMDQyNjY3LTk0LjkzMzMzMyAxMzMuOTczMzMzLTIzMC4xMDEzMzN6IG0yNDEuMTUyIDIxNC4zNTczMzRhMzc2LjU3NiAzNzYuNTc2IDAgMCAwIDIzOS4xODkzMzQtMjE0LjM1NzMzNGgtMTYzLjQ1NmE1MTAuOTMzMzMzIDUxMC45MzMzMzMgMCAwIDEtNzUuNjkwNjY3IDIxNC4zMTQ2Njd6TTE2NS42MzIgNjU3LjMyMjY2N2EzNzYuMjM0NjY3IDM3Ni4yMzQ2NjcgMCAwIDAgMjM5LjE4OTMzMyAyMTQuMzU3MzMzIDUxMC45MzMzMzMgNTEwLjkzMzMzMyAwIDAgMS03NS42OTA2NjYtMjE0LjM1NzMzM0gxNjUuNjMyeiIgZmlsbD0iIzJjMmMyYyIgcC1pZD0iMTIxNzYiPjwvcGF0aD48L3N2Zz4=";
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