#include "badgecpp/render.hpp"
#include "badgecpp/color.hpp"
#include "badgecpp/font.hpp"
#include "badgecpp/xml.hpp"
#include <optional>
#include <string>
#include <sys/types.h>
namespace {
    using namespace badge;

    static constexpr const uint HORIZ_PADDING = 5;
    static constexpr const uint LOGO_LABEL_PADDING = 3;
    static constexpr const uint LOGO_HEIGHT = 14;
    static constexpr const uint DEFAULT_LOGO_WIDTH = 14;
    static const std::string DEFAULT_LABEL_COLOR = "#555";
    static const std::string DEFAULT_MESSAGE_COLOR = "#4c1";


    static const Font &FONT = Fonts::getDefault();
    static const std::string FONT_FAMILY = "Verdana,Geneva,DejaVu Sans,sans-serif";

    // https://github.com/badges/shields/pull/1132
    static const uint FONT_SIZE_UP = 10;                  ///< 放大10倍
    static const std::string FONT_SIZE_DOWN = "scale(.1)";///< 缩小到10%

}// namespace
namespace badge {


    Render::Render(const Badge &badge) : badge(badge) {
    }
    Xml Render::render() {

        hasLogo = bool(badge.logo_);
        hasLabel = badge.label_ || badge.label_color_;
        hasMessage = badge.message_ || badge.message_color_;

        logo_width = badge.logo_ ? (badge.logo_width_ ? *badge.logo_width_ : DEFAULT_LOGO_WIDTH) : 0;

        label_width = get_str_width(badge.label_);

        label_margin =
                hasLogo && hasLabel ? 1 + logo_width + LOGO_LABEL_PADDING
                : hasLabel          ? 1
                                    : 0;

        left_width =
                hasLogo && hasLabel ? HORIZ_PADDING + logo_width + LOGO_LABEL_PADDING + label_width + HORIZ_PADDING
                : hasLogo           ? HORIZ_PADDING + logo_width + HORIZ_PADDING
                : hasLabel          ? HORIZ_PADDING + label_width + HORIZ_PADDING
                                    : 0;

        message_width = get_str_width(badge.message_);

        message_margin = left_width - (hasMessage ? 1 : 0);

        right_width =
                hasMessage ? HORIZ_PADDING + message_width + HORIZ_PADDING
                           : 0;

        height = get_height();
        width = left_width + right_width > 0 ? left_width + right_width : height;

        accessible_test = get_accessible_text();

        idSuffix = badge.id_suffix_ ? *badge.id_suffix_ : "";


        Xml svg("svg", {
                               {"xmlns", "http://www.w3.org/2000/svg"},
                               {"xmlns:xlink", "http://www.w3.org/1999/xlink"},
                               {"width", std::to_string(width)},
                               {"height", std::to_string(height)},
                       });

        //if (!badge.link_)
        {
            svg.addAttr("role", "img");
            svg.addAttr("aria-label", accessible_test);
        }

        add_content(svg);
        return svg;
    }
    uint Render::get_str_width(const std::optional<std::string> &str) const {
        if (!str) return 0;
        auto width = uint(FONT.widthOfString(*str));
        return width % 2 ? width : (width + 1);
    }
    std::string Render::get_accessible_text() const {
        return badge.label_ && badge.message_ ? *badge.label_ + ": " + *badge.message_
               : badge.label_                 ? *badge.label_ + ":"
               : badge.message_               ? *badge.message_
                                              : "";
    }
    Xml Render::getClipPathElement(int rx) const {
        return {
                "clipPath",
                {{"id", "r" + idSuffix}},
                Xml{
                        "rect",
                        {
                                {"width", std::to_string(width)},
                                {"height", std::to_string(height)},
                                {"rx", std::to_string(rx)},
                                {"fill", "#fff"},
                        },
                }};
    }
    Xml Render::getBackgroundGroupElement(bool withGradient, Xml::Attrs attrs) const {
        Xml node{
                "g",
                std::move(attrs),
                //left react
                Xml{
                        "rect",
                        {
                                {"width", std::to_string(left_width)},
                                {"height", std::to_string(height)},
                                {"fill", badge.label_color_ ? *badge.label_color_ : DEFAULT_LABEL_COLOR},
                        },
                },
                //right react
                Xml{
                        "rect",
                        {
                                {"x", std::to_string(left_width)},
                                {"width", std::to_string(right_width)},
                                {"height", std::to_string(height)},
                                {"fill", badge.message_color_ ? *badge.message_color_ : DEFAULT_MESSAGE_COLOR},
                        },
                },
        };
        if (withGradient) {
            node.addContent(Xml{
                    "rect",
                    {
                            {"width", std::to_string(width)},
                            {"height", std::to_string(height)},
                            {"fill", "url(#s" + idSuffix + ")"},
                    },
            });
        }

        return node;
    }
    Xml Render::getForegroundGroupElement() const {
        return {
                "g",
                {
                        {"fill", "#fff"},// TODO 自定义颜色
                        {"text-anchor", "middle"},
                        {"font-family", FONT_FAMILY},
                        {"text-rendering", "geometricPrecision"},
                        {"font-size", std::to_string(FONT_SIZE_UP * FONT.size())},
                },
                getLogoElement(),
                getLabelElement(),
                getMessageElement(),
        };
    }
    Xml Render::getLogoElement() const {
        if (!badge.logo_) return {};
        return {
                "image",
                {
                        {"x", std::to_string(HORIZ_PADDING)},
                        {"y", std::to_string(0.5 * (height - LOGO_HEIGHT))},
                        {"width", std::to_string(logo_width)},
                        {"height", std::to_string(LOGO_HEIGHT)},
                        {"xlink:href", *badge.logo_},
                },
        };
    }
    Xml Render::getTextElement(uint left_margin, std::optional<std::string> content, std::string color, uint text_width,
                               std::optional<std::string> link, uint link_width) const {
        if (!content) return {};
        const auto [textColor, shadowColor] = Color{color}.getColorHexPairForBackground();

        const auto x = FONT_SIZE_UP * (left_margin + 0.5 * text_width + HORIZ_PADDING);
        const auto vertical_margin = get_vertical_margin();

        Xml node;

        if (link) {
            node.setName("a");
            node.addAttr("target", "_blank");
            node.addAttr("xlink:href", *link);
            node.addContent(Xml{
                    "rect",
                    {
                            {"x", std::to_string(left_margin > 1 ? left_margin + 1 : 0)},
                            {"width", std::to_string(link_width)},
                            {"height", std::to_string(height)},
                            {"fill", "rgba(0,0,0,0)"},
                    }});
        }

        node.addContent(Xml{
                // text
                "text",
                {
                        {"x", std::to_string(x)},
                        {"y", std::to_string(140 + vertical_margin)},
                        {"fill", textColor},
                        {"transform", FONT_SIZE_DOWN},
                        {"textLength", std::to_string(FONT_SIZE_UP * text_width)},
                },
                *content,
        });
        if (text_has_shadow()) {
            node.addContent(Xml{
                    // shadowText
                    "text",
                    {
                            {"aria-hidden", "true"},
                            {"x", std::to_string(x)},
                            {"y", std::to_string(150 + vertical_margin)},
                            {"fill", shadowColor},
                            {"fill-opacity", ".3"},
                            {"transform", FONT_SIZE_DOWN},
                            {"textLength", std::to_string(FONT_SIZE_UP * text_width)},
                    },
            });
        }


        return node;
    }
    Xml Render::getLabelElement() const {
        return getTextElement(
                label_margin,
                badge.label_,
                badge.label_color_ ? *badge.label_color_ : DEFAULT_LABEL_COLOR,
                label_width,
                left_link,
                left_width);
    }
    Xml Render::getMessageElement() const {
        return getTextElement(
                message_margin,
                badge.message_,
                badge.message_color_ ? *badge.message_color_ : DEFAULT_MESSAGE_COLOR,
                message_width,
                right_link,
                right_width);
    }


    std::unique_ptr<Render> Render::create(const Badge &badge) {
        switch (badge.style_) {
                // clang-format off
            case FLAT:         return std::make_unique<FlatRender>(badge);
            case FLAT_SQUARE:  //TODO
            case PLASTIC:      //TODO
            case FOR_THE_BADGE://TODO
            case SOCIAL:       //TODO
                break;
                // clang-format on
        }
        throw std::runtime_error("[badgecpp::Render::create] Unsupported style: " + std::to_string(badge.style_));
    }


}// namespace badge