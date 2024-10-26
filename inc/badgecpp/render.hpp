/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_RENDER__HPP_GUARD
#define BADGECPP_RENDER__HPP_GUARD
#include "badgecpp/badge.hpp"
#include "badgecpp/xml.hpp"
#include <memory>
#include <optional>
#include <string>
namespace badge {
    class Render {
    public:
        explicit Render(const Badge &badge);
        virtual ~Render() {}
        Xml render();

        static std::unique_ptr<Render> create(const Badge &badge);

    protected:
        virtual uint get_height() const = 0;
        virtual uint get_vertical_margin() const = 0;
        virtual bool text_has_shadow() const = 0;
        virtual uint get_str_width(const std::optional<std::string> &str) const;
        virtual void add_content(Xml &xml) const = 0;
        virtual std::string get_accessible_text() const;


    protected:// helpers functions
        Xml getClipPathElement(int rx) const;
        Xml getBackgroundGroupElement(bool withGradient, Xml::Attrs attrs) const;
        Xml getForegroundGroupElement() const;
        Xml getLogoElement() const;
        Xml getTextElement(uint left_margin, std::optional<std::string> content, std::string color, uint width,
                           std::optional<std::string> link, uint link_width) const;
        Xml getLabelElement() const;
        Xml getMessageElement() const;


    protected:
        const Badge &badge;
        bool hasLogo;
        bool hasLabel;
        bool hasMessage;
        uint logo_width;
        uint label_margin;
        uint label_width;
        uint message_width;
        uint message_margin;
        uint left_width;
        uint right_width;
        uint height;
        uint width;
        std::optional<std::string> body_link;
        std::optional<std::string> left_link;
        std::optional<std::string> right_link;
        std::string accessible_test;
        std::string idSuffix;
    };
    class FlatRender final : public Render {
    protected:
        uint get_height() const override;
        void add_content(Xml &xml) const override;
        uint get_vertical_margin() const override;
        bool text_has_shadow() const override;

    public:
        using Render::Render;
    };
}// namespace badge
#endif// BADGECPP_RENDER__HPP_GUARD
