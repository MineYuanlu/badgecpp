#include "badgecpp/badge.hpp"
#include "badgecpp/reg.h"
#include <badgecpp/font.hpp>
#include <fstream>
#include <iostream>
int main() {
    using namespace badge;
    Badge svg{
            "label",
            std::nullopt,
            "message",
            std::nullopt,
            badge::FLAT,
            std::nullopt,
            std::nullopt,
            "id",
    };
    Xml html{
            "html",
            {{"lang", "en"}},
            Xml{
                    "body",
                    Xml{"h1", "badge:"},
                    svg.makeBadgeXml(),
            },
    };
    std::string file = "badge.html";
    std::ofstream fout(file);
    fout << html.render();
    return 0;
}