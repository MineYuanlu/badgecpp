/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_RENDER_HPP_GUARD
#define BADGECPP_RENDER_HPP_GUARD
#include "badgecpp/badge.hpp"
#include "badgecpp/xml.hpp"
#include <memory>
#include <optional>
#include <string>
namespace badge {
    class Render {
    protected:
        enum class TextType {
            LABEL,
            MESSAGE,
        };

    public:
        explicit Render(const Badge &badge);
        virtual ~Render() {}
        virtual Xml render();

        static std::unique_ptr<Render> create(const Badge &badge);

    protected:
        virtual void calcValues();
        [[nodiscard]] virtual Xml renderSvg() const;
        [[nodiscard]] virtual unsigned int get_height() const = 0;
        [[nodiscard]] virtual unsigned int get_vertical_margin() const = 0;
        [[nodiscard]] virtual bool text_has_shadow() const = 0;
        [[nodiscard]] virtual unsigned int get_str_width(const std::optional<std::string> &str, TextType tt) const;
        [[nodiscard]] virtual Xml get_content() const = 0;
        [[nodiscard]] virtual std::string get_accessible_text() const;
        [[nodiscard]] virtual unsigned int get_logo_width() const;


    protected:// helpers functions
        [[nodiscard]] Xml getClipPathElement(int rx) const;
        [[nodiscard]] Xml getBackgroundGroupElement(bool withGradient, Xml::Attrs attrs) const;
        [[nodiscard]] Xml getForegroundGroupElement() const;
        [[nodiscard]] Xml getLogoElement(unsigned int horizPadding, unsigned int badgeHeight) const;
        [[nodiscard]] Xml getTextElement(unsigned int left_margin, std::optional<std::string> content, const std::string &color, unsigned int width,
                                         std::optional<std::string> link, unsigned int link_width) const;
        [[nodiscard]] Xml getLabelElement() const;
        [[nodiscard]] Xml getMessageElement() const;


    protected:
        const Badge &badge;
        bool hasLogo;
        bool hasLabel;
        bool hasMessage;
        unsigned int logo_width;
        unsigned int label_margin;
        unsigned int label_width;
        unsigned int message_width;
        unsigned int message_margin;
        unsigned int left_width;
        unsigned int right_width;
        unsigned int height;
        unsigned int width;
        std::optional<std::string> body_link;
        std::optional<std::string> left_link;
        std::optional<std::string> right_link;
        std::string accessible_test;
        std::string idSuffix;
    };
}// namespace badge
#endif// BADGECPP_RENDER_HPP_GUARD
