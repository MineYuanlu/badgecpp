#include "badgecpp/render.hpp"

namespace badge {
    uint FlatRender::get_height() const {
        return 20;
    }
    uint FlatRender::get_vertical_margin() const {
        return 0;
    }
    bool FlatRender::text_has_shadow() const {
        return true;
    }
    void FlatRender::add_content(Xml &xml) const {
        xml.addContent(Xml{
                "linearGradient",
                {
                        {"id", "s" + idSuffix},
                        {"x2", "0"},
                        {"y2", "100%"},
                },
                Xml{
                        "stop",
                        {
                                {"offset", "0"},
                                {"stop-color", "#bbb"},
                                {"stop-opacity", ".1"},
                        },
                },
                Xml{
                        "stop",
                        {
                                {"offset", "1"},
                                {"stop-opacity", ".1"},
                        },
                },
        });
        xml.addContent(getClipPathElement(3));
        xml.addContent(getBackgroundGroupElement(
                true,
                {{"clip-path", "url(#r" + idSuffix + ")"}}));
        xml.addContent(getForegroundGroupElement());
    }
}// namespace badge