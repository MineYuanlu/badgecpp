#include "badgecpp/render.hpp"
#include "badgecpp/color.hpp"
#include "badgecpp/font.hpp"
#include "badgecpp/render.impl.hpp"
#include "badgecpp/util.hpp"
#include "badgecpp/xml.hpp"
#include <optional>
#include <stdexcept>
#include <string>
#include <sys/types.h>
namespace {
    using namespace badge;

    constexpr unsigned int HORIZ_PADDING = 5;
    constexpr unsigned int LOGO_LABEL_PADDING = 3;
    constexpr unsigned int LOGO_HEIGHT = 14;
    constexpr unsigned int DEFAULT_LOGO_WIDTH = 14;
    const std::string DEFAULT_LABEL_COLOR = "#555";
    const std::string DEFAULT_MESSAGE_COLOR = "#4c1";


    const Font &FONT = Fonts::get("verdana-11px-normal");
    const std::string FONT_FAMILY = "Verdana,Geneva,DejaVu Sans,sans-serif";

    // https://github.com/badges/shields/pull/1132
    constexpr unsigned int FONT_SIZE_UP = 10;      ///< 放大10倍
    const std::string FONT_SIZE_DOWN = "scale(.1)";///< 缩小到10%

}// namespace
namespace badge {


    Render::Render(const Badge &badge) : badge(badge) {
    }
    Xml Render::render() {
        calcValues();
        return renderSvg();
    }
    void Render::calcValues() {
        hasLogo = static_cast<bool>(badge.logo_);
        hasLabel = bool(badge.label_);
        hasMessage = bool(badge.message_);

        logo_width = get_logo_width();

        label_width = get_str_width(badge.label_, TextType::LABEL);

        label_margin =
                hasLogo && hasLabel ? 1 + logo_width + LOGO_LABEL_PADDING
                : hasLabel          ? 1
                                    : 0;

        left_width =
                hasLogo && hasLabel ? HORIZ_PADDING + logo_width + LOGO_LABEL_PADDING + label_width + HORIZ_PADDING
                : hasLogo           ? HORIZ_PADDING + logo_width + HORIZ_PADDING
                : hasLabel          ? HORIZ_PADDING + label_width + HORIZ_PADDING
                                    : 0;

        message_width = get_str_width(badge.message_, TextType::MESSAGE);

        message_margin = left_width - (hasMessage && left_width > 0 ? 1 : 0);

        right_width =
                hasMessage ? HORIZ_PADDING + message_width + HORIZ_PADDING
                           : 0;

        height = get_height();
        width = left_width + right_width > 0 ? left_width + right_width : 0;

        accessible_test = get_accessible_text();

        idSuffix = badge.id_suffix_ ? *badge.id_suffix_ : "";
    }
    Xml Render::renderSvg() const {
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

        svg.addContent(get_content());
        return svg;
    }
    unsigned int Render::get_str_width(const std::optional<std::string> &str, [[maybe_unused]] TextType tt) const {
        if (!str) return 0;
        auto width = static_cast<unsigned int>(FONT.widthOfString(*str));
        return width % 2 ? width : (width + 1);
    }
    std::string Render::get_accessible_text() const {
        return badge.label_ && badge.message_ ? *badge.label_ + ": " + *badge.message_
               : badge.label_                 ? *badge.label_ + ":"
               : badge.message_               ? *badge.message_
                                              : "";
    }
    unsigned int Render::get_logo_width() const {
        return badge.logo_ ? (badge.logo_width_ ? *badge.logo_width_ : DEFAULT_LOGO_WIDTH) : 0;
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
                        {"fill", "#f0f"},// TODO 自定义颜色
                        {"text-anchor", "middle"},
                        {"font-family", FONT_FAMILY},
                        {"text-rendering", "geometricPrecision"},
                        {"font-size", std::to_string(FONT_SIZE_UP * FONT.size())},
                },
                getLogoElement(HORIZ_PADDING, height),
                getLabelElement(),
                getMessageElement(),
        };
    }
    Xml Render::getLogoElement(unsigned int horizPadding, unsigned int badgeHeight) const {
        if (!badge.logo_) return {};
        return {
                "image",
                {
                        {"x", std::to_string(horizPadding)},
                        {"y", std::to_string(0.5 * (badgeHeight - LOGO_HEIGHT))},
                        {"width", std::to_string(logo_width)},
                        {"height", std::to_string(LOGO_HEIGHT)},
                        {"xlink:href", *badge.logo_},
                },
        };
    }
    Xml Render::getTextElement(unsigned int left_margin, std::optional<std::string> content, const std::string &color, unsigned int text_width,
                               std::optional<std::string> link, unsigned int link_width) const {
        if (!content) return {};
        const auto [textColor, shadowColor] = Color{color}.getColorHexPairForBackground();

        const auto x = badge::dtos(FONT_SIZE_UP * (left_margin + 0.5 * text_width + HORIZ_PADDING));
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
                        {"x", x},
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
                            {"x", x},
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
            case FLAT_SQUARE:  return std::make_unique<FlatSquareRender>(badge);
            case PLASTIC:      return std::make_unique<PlasticRender>(badge);
            case FOR_THE_BADGE:return std::make_unique<ForTheBadgeRender>(badge);
            case SOCIAL:       return std::make_unique<SocialRender>(badge);
                // clang-format on
            default:
                break;
        }
        throw std::runtime_error("[badgecpp::Render::create] Unsupported style: " + std::to_string(badge.style_));
    }


}// namespace badge


namespace badge {
    unsigned int FlatSquareRender::get_height() const {
        return 20;
    }
    unsigned int FlatSquareRender::get_vertical_margin() const {
        return 0;
    }
    bool FlatSquareRender::text_has_shadow() const {
        return false;
    }
    Xml FlatSquareRender::get_content() const {
        return {
                "",
                getBackgroundGroupElement(
                        false, {
                                       {"shape-rendering", "crispEdges"},
                               }),
                getForegroundGroupElement(),
        };
    }
}// namespace badge

namespace badge {
    unsigned int FlatRender::get_height() const {
        return 20;
    }
    unsigned int FlatRender::get_vertical_margin() const {
        return 0;
    }
    bool FlatRender::text_has_shadow() const {
        return true;
    }
    Xml FlatRender::get_content() const {
        return {
                "",
                Xml{
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
                },
                getClipPathElement(3),
                getBackgroundGroupElement(
                        true,
                        {{"clip-path", "url(#r" + idSuffix + ")"}}),
                getForegroundGroupElement(),
        };
    }
}// namespace badge


namespace badge {
    unsigned int PlasticRender::get_height() const {
        return 18;
    }
    unsigned int PlasticRender::get_vertical_margin() const {
        return -10;
    }
    bool PlasticRender::text_has_shadow() const {
        return true;
    }
    Xml PlasticRender::get_content() const {
        return {
                "",
                Xml{
                        "linearGradient",
                        {
                                {"id", 's' + idSuffix},
                                {"x2", "0"},
                                {"y2", "100%"},
                        },
                        Xml{
                                "stop",
                                {
                                        {"offset", "0"},
                                        {"stop-color", "#fff"},
                                        {"stop-opacity", ".7"},
                                },
                        },
                        Xml{
                                "stop",
                                {
                                        {"offset", ".1"},
                                        {"stop-color", "#aaa"},
                                        {"stop-opacity", ".1"},
                                },
                        },
                        Xml{
                                "stop",
                                {
                                        {"offset", ".9"},
                                        {"stop-color", "#000"},
                                        {"stop-opacity", ".3"},
                                },
                        },
                        Xml{
                                "stop",
                                {
                                        {"offset", "1"},
                                        {"stop-color", "#000"},
                                        {"stop-opacity", ".5"},
                                },
                        },
                },
                getClipPathElement(4),
                getBackgroundGroupElement(
                        true, {
                                      {"clip-path", "url(#r" + idSuffix + ')'},
                              }),
                getForegroundGroupElement(),
        };
    }
}// namespace badge


namespace {
    using namespace badge;

    constexpr unsigned int socialExternalHeigth = 20;
    constexpr unsigned int socialInternalHeight = 19;
    constexpr unsigned int socialLabelHorizPadding = 5;
    constexpr unsigned int socialMessageHorizPadding = 4;
    constexpr unsigned int socialHorizGutter = 6;

    const std::string SOCIAL_FONT_FAMILY = "Helvetica Neue,Helvetica,Arial,sans-serif";
    const Font &SOCIAL_FONT = Fonts::get("helvetica-11px-bold");
}// namespace

namespace badge {
    unsigned int SocialRender::get_height() const {
        return socialExternalHeigth;
    }
    unsigned int SocialRender::get_vertical_margin() const {
        throw std::logic_error("[badgecpp::SocialRender::get_vertical_margin] Not implemented");
    }
    bool SocialRender::text_has_shadow() const {
        throw std::logic_error("[badgecpp::SocialRender::text_has_shadow] Not implemented");
    }
    unsigned int SocialRender::get_str_width(const std::optional<std::string> &str, [[maybe_unused]] TextType tt) const {
        if (!str) return 0;
        auto width = static_cast<unsigned int>(SOCIAL_FONT.widthOfString(*str));
        return width % 2 ? width : (width + 1);
    }
    Xml SocialRender::render() {
        calcValues();

        label_rect_width =
                hasLogo && hasLabel ? socialLabelHorizPadding + logo_width + LOGO_LABEL_PADDING + label_width + socialLabelHorizPadding
                : hasLabel          ? socialLabelHorizPadding + label_width + socialLabelHorizPadding
                : hasLogo           ? socialLabelHorizPadding + logo_width + socialLabelHorizPadding
                                    : 0;
        left_width = label_rect_width + 1;
        message_rect_width = socialMessageHorizPadding + message_width + socialMessageHorizPadding;
        right_width = hasMessage ? socialHorizGutter + message_rect_width : 0;

        width = left_width + right_width > 0 ? left_width + right_width : height;

        return Render::renderSvg();
    }
    Xml SocialRender::get_content() const {
        std::string style_str = "a:hover #llink";
        style_str += idSuffix;
        style_str += "{fill:url(#b";
        style_str += idSuffix;
        style_str += ");stroke:#ccc}a:hover #rlink";
        style_str += idSuffix;
        style_str += "{fill:#4183c4}";
        Xml style{"style", style_str};

        Xml gradients{
                "",
                Xml{
                        "linearGradient",
                        {
                                {"id", "a" + idSuffix},
                                {"x2", "0"},
                                {"y2", "100%"},
                        },
                        Xml{
                                "stop",
                                {
                                        {"offset", "0"},
                                        {"stop-color", "#fcfcfc"},
                                        {"stop-opacity", "0"},
                                },
                        },
                        Xml{
                                "stop",
                                {
                                        {"offset", "1"},
                                        {"stop-opacity", ".1"},
                                },
                        },
                },
                Xml{
                        "linearGradient",
                        {
                                {"id", "b" + idSuffix},
                                {"x2", "0"},
                                {"y2", "100%"},
                        },
                        Xml{
                                "stop",
                                {
                                        {"offset", "0"},
                                        {"stop-color", "#ccc"},
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
                },
        };
        Xml labelRect1{
                "rect",
                {
                        {"x", "0.5"},
                        {"y", "0.5"},
                        {"width", std::to_string(label_rect_width)},
                        {"height", std::to_string(socialInternalHeight)},
                        {"rx", "2"},
                        {"stroke", "none"},
                        {"fill", "#fcfcfc"},
                },
        };
        Xml labelRect2{
                "rect",
                {
                        {"id", "llink" + idSuffix},
                        {"stroke", "#d5d5d5"},
                        {"fill", "url(#a" + idSuffix + ")"},
                        {"x", ".5"},
                        {"y", ".5"},
                        {"width", std::to_string(label_rect_width)},
                        {"height", std::to_string(socialInternalHeight)},
                        {"rx", "2"},
                },
        };
        Xml backgroundGroup{
                "g",
                {{"stroke", "#d5d5d5"}},
                std::move(labelRect1),
                getMessageBubble(),
                std::move(labelRect2),
        };
        Xml foregroundGroup{
                "g",
                {
                        {"aria-hidden", body_link ? "true" : "false"},
                        {"fill", "#333"},
                        {"text-anchor", "middle"},
                        {"font-family", SOCIAL_FONT_FAMILY},
                        {"text-rendering", "geometricPrecision"},
                        {"font-weight", "700"},
                        {"font-size", "110px"},
                        {"line-height", "14px"},
                },
                getLabelText(),
                getMessageText(),
        };
        return {
                "",
                std::move(style),
                std::move(gradients),
                std::move(backgroundGroup),
                getLogoElement(socialLabelHorizPadding, height),
                std::move(foregroundGroup),
        };
    }

    Xml SocialRender::getMessageBubble() const {
        if (!hasMessage) return {};
        const auto messageBubbleNotchX = label_rect_width + socialHorizGutter;
        const auto messageBubbleMainX = messageBubbleNotchX + 0.5;
        return {
                "",
                Xml{
                        "rect",
                        {
                                {"x", std::to_string(messageBubbleMainX)},
                                {"y", "0.5"},
                                {"width", std::to_string(message_rect_width)},
                                {"height", std::to_string(socialInternalHeight)},
                                {"rx", "2"},
                                {"fill", "#fafafa"},
                        },
                },
                Xml{
                        "rect",
                        {
                                {"x", std::to_string(messageBubbleNotchX)},
                                {"y", "7.5"},
                                {"width", "0.5"},
                                {"height", "5"},
                                {"stroke", "#fafafa"},
                        },
                },
                Xml{
                        "path",
                        {
                                {"d", "M" + std::to_string(messageBubbleMainX) + " 6.5 l-3 3v1 l3 3"},
                                {"fill", "#fafafa"},
                                {"stroke", "d5d5d5"},
                        },
                },
        };
    }
    Xml SocialRender::getLabelText() const {
        if (!badge.label_) return {};

        const auto labelTextX = dtos(FONT_SIZE_UP * (hasLogo
                                                             ? logo_width + LOGO_LABEL_PADDING + label_width / 2.0 + socialLabelHorizPadding
                                                             : label_width / 2.0 + socialLabelHorizPadding));
        const auto labelTextLength = FONT_SIZE_UP * label_width;
        const auto shouldWarpLink = left_link && !body_link;

        Xml shadow{
                "text",
                {
                        {"aria-hidden", "true"},
                        {"x", labelTextX},
                        {"y", "150"},
                        {"fill", "#fff"},
                        {"transform", FONT_SIZE_DOWN},
                        {"textLength", std::to_string(labelTextLength)},
                },
                *badge.label_,
        };
        Xml text{
                "text",
                {
                        {"x", labelTextX},
                        {"y", "140"},
                        {"transform", FONT_SIZE_DOWN},
                        {"textLength", std::to_string(labelTextLength)},
                },
                *badge.label_,
        };
        if (shouldWarpLink) {
            return Xml{
                    "a",
                    {{"target", "_blank"}, {"xlink:href", *left_link}},
                    std::move(shadow),
                    std::move(text),
            };
        } else {
            return Xml{
                    "",
                    std::move(shadow),
                    std::move(text),
            };
        }
    }
    Xml SocialRender::getMessageText() const {
        if (!hasMessage) return {};
        const auto messageTextX = dtos(FONT_SIZE_UP * (label_rect_width + socialHorizGutter + message_rect_width / 2.0));
        const auto messageTextLength = FONT_SIZE_UP * message_width;


        Xml shadow{
                "text",
                {
                        {"aria-hidden", "true"},
                        {"x", messageTextX},
                        {"y", "150"},
                        {"fill", "#fff"},
                        {"transform", FONT_SIZE_DOWN},
                        {"textLength", std::to_string(messageTextLength)},
                },
                *badge.message_,
        };
        Xml text{
                "text",
                {
                        {"id", "rlink" + idSuffix},
                        {"x", messageTextX},
                        {"y", "140"},
                        {"transform", FONT_SIZE_DOWN},
                        {"textLength", std::to_string(messageTextLength)},
                },
                *badge.message_,
        };

        if (right_link) {
            Xml rect{
                    "rect",
                    {
                            {"width", std::to_string(message_rect_width + 1)},
                            {"x", std::to_string(label_rect_width + socialHorizGutter)},
                            {"height", std::to_string(socialInternalHeight + 1)},
                            {"fill", "rgba(0,0,0,0)"},
                    },
            };
            return Xml{
                    "a",
                    {{"target", "_blank"}, {"xlink:href", *right_link}},
                    std::move(rect),
                    std::move(shadow),
                    std::move(text),
            };
        } else {
            return Xml{
                    "",
                    std::move(shadow),
                    std::move(text),
            };
        }
    }
}// namespace badge


namespace {
    constexpr unsigned int ftbFontSize = 10;
    constexpr unsigned int ftbBadgeHeight = 28;
    constexpr unsigned int ftbTextMargin = 12;
    constexpr unsigned int ftbLogoMargin = 9;
    constexpr unsigned int ftbLogoTextGutter = 6;
    constexpr double ftbLetterSpacing = 1.25;

    const Font &ftbLabelFont = Fonts::get("verdana-10px-normal");
    const Font &ftbMessageFont = Fonts::get("verdana-10px-bold");
}// namespace

namespace badge {

    [[nodiscard]] unsigned int ForTheBadgeRender::get_height() const {
        return ftbBadgeHeight;
    }
    [[nodiscard]] unsigned int ForTheBadgeRender::get_vertical_margin() const {
        throw std::logic_error("[badgecpp::ForTheBadgeRender::get_vertical_margin] Not implemented");
    }
    [[nodiscard]] bool ForTheBadgeRender::text_has_shadow() const {
        throw std::logic_error("[badgecpp::ForTheBadgeRender::text_has_shadow] Not implemented");
    }

    unsigned int ForTheBadgeRender::get_str_width(const std::optional<std::string> &str, TextType tt) const {
        if (!str) return 0;

        switch (tt) {
            case Render::TextType::LABEL: {
                auto u32str = Font::toU32String(up_label);
                return ftbLabelFont.widthOfString(u32str) + ftbLetterSpacing * u32str.length();
            }
            case Render::TextType::MESSAGE: {
                auto u32str = Font::toU32String(up_message);
                return ftbMessageFont.widthOfString(u32str) + ftbLetterSpacing * u32str.length();
            }
        }
        throw std::logic_error("[badgecpp::ForTheBadgeRender::get_str_width] Unsupported text type");
    }
    Xml ForTheBadgeRender::render() {

        if (badge.label_) up_label = toUpperCase(*badge.label_);
        if (badge.message_) up_message = toUpperCase(*badge.message_);

        calcValues();


        label_text_min_x = hasLogo ? ftbLogoMargin + logo_width + ftbLogoTextGutter
                                   : ftbTextMargin;

        if (hasLabel || hasLogo) {
            label_rect_width = hasLabel ? label_text_min_x + label_width + ftbTextMargin
                                        : ftbLogoMargin + logo_width + ftbLogoMargin;
            message_text_min_x = label_rect_width + ftbTextMargin;
            message_rect_width = hasMessage ? ftbTextMargin + message_width + ftbTextMargin : 0;
        } else if (hasLogo) {
            label_rect_width = 0;
            message_text_min_x = ftbTextMargin + logo_width + ftbLogoTextGutter;
            message_rect_width = hasMessage ? ftbTextMargin + logo_width + ftbLogoTextGutter + message_width + ftbTextMargin : 0;
        } else {
            label_rect_width = 0;
            message_text_min_x = ftbTextMargin;
            message_rect_width = hasMessage ? ftbTextMargin + message_width + ftbTextMargin : 0;
        }

        width = label_rect_width + message_rect_width;

        return Render::renderSvg();
    }


    Xml ForTheBadgeRender::get_content() const {

        Xml foregroundGroup{
                "g",
                {
                        {"fill", "#fff"},
                        {"text-anchor", "middle"},
                        {"font-family", FONT_FAMILY},
                        {"text-rendering", "geometricPrecision"},
                        {"font-size", std::to_string(FONT_SIZE_UP * ftbFontSize)},
                },
        };
        if (hasLogo) foregroundGroup.addContent(getLogoElement(ftbLogoMargin, ftbBadgeHeight));
        if (hasLabel) foregroundGroup.addContent(getLabelElement());
        if (hasMessage) foregroundGroup.addContent(getMessageElement());
        Xml backgroundGroup{
                "g",
                {{"shape-rendering", "crispEdges"}},
        };
        if (hasLabel || hasLogo) {
            // label background
            backgroundGroup.addContent(Xml{
                    "rect",
                    {
                            {"width", std::to_string(label_rect_width)},
                            {"height", std::to_string(ftbBadgeHeight)},
                            {"fill", badge.label_color_ ? *badge.label_color_ : DEFAULT_LABEL_COLOR},
                    },
            });
            // message background
            if (hasMessage) {
                backgroundGroup.addContent(Xml{
                        "rect",
                        {
                                {"x", std::to_string(label_rect_width)},
                                {"width", std::to_string(message_rect_width)},
                                {"height", std::to_string(ftbBadgeHeight)},
                                {"fill", badge.message_color_ ? *badge.message_color_ : DEFAULT_MESSAGE_COLOR},
                        },
                });
            }
        } else if (hasMessage) {
            // message background
            backgroundGroup.addContent(Xml{
                    "rect",
                    {
                            {"width", std::to_string(message_rect_width)},
                            {"height", std::to_string(ftbBadgeHeight)},
                            {"fill", badge.message_color_ ? *badge.message_color_ : DEFAULT_MESSAGE_COLOR},
                    },
            });
        }
        return {
                "",
                std::move(backgroundGroup),
                std::move(foregroundGroup),
        };
    }

    Xml ForTheBadgeRender::getLabelElement() const {
        const auto textColor = Color(badge.label_color_ ? *badge.label_color_ : DEFAULT_LABEL_COLOR).getColorHexPairForBackground().first;
        const auto midX = label_text_min_x + 0.5 * label_width;

        Xml text{
                "text",
                {
                        {"transform", FONT_SIZE_DOWN},
                        {"x", dtos(FONT_SIZE_UP * midX)},
                        {"y", "175"},
                        {"textLength", dtos(FONT_SIZE_UP * label_width)},
                        {"fill", textColor},
                },
                up_label,
        };
        if (left_link) {
            return {
                    "a",
                    {{"target", "_blank"}, {"xlink:href", *left_link}},
                    Xml{
                            "rect",
                            {
                                    {"width", std::to_string(label_rect_width)},
                                    {"height", std::to_string(ftbBadgeHeight)},
                                    {"fill", "rgba(0,0,0,0)"},
                            },
                    },
                    std::move(text),
            };
        } else {
            return text;
        }
    }
    Xml ForTheBadgeRender::getMessageElement() const {
        const auto textColor = Color(badge.message_color_ ? *badge.message_color_ : DEFAULT_MESSAGE_COLOR).getColorHexPairForBackground().first;
        const auto midX = message_text_min_x + 0.5 * message_width;

        Xml text{
                "text",
                {
                        {"transform", FONT_SIZE_DOWN},
                        {"x", dtos(FONT_SIZE_UP * midX)},
                        {"y", "175"},
                        {"textLength", dtos(FONT_SIZE_UP * message_width)},
                        {"fill", textColor},
                        {"font-weight", "bold"},
                },
                up_message,
        };
        if (right_link) {
            return {
                    "a",
                    {{"target", "_blank"}, {"xlink:href", *right_link}},
                    Xml{
                            "rect",
                            {
                                    {"width", std::to_string(message_rect_width)},
                                    {"height", std::to_string(ftbBadgeHeight)},
                                    {"x", std::to_string(label_rect_width)},
                                    {"fill", "rgba(0,0,0,0)"},
                            },
                    },
                    std::move(text),
            };
        } else {
            return text;
        }
    }

}// namespace badge