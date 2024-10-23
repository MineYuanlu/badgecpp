/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_RENDER_IMPL_HPP_GUARD
#define BADGECPP_RENDER_IMPL_HPP_GUARD

#include "badgecpp/render.hpp"

namespace badge {
    class FlatRender final : public Render {
    protected:
        [[nodiscard]] unsigned int get_height() const override;
        [[nodiscard]] Xml get_content() const override;
        [[nodiscard]] unsigned int get_vertical_margin() const override;
        [[nodiscard]] bool text_has_shadow() const override;

    public:
        using Render::Render;
    };
    class FlatSquareRender final : public Render {
    protected:
        [[nodiscard]] unsigned int get_height() const override;
        [[nodiscard]] Xml get_content() const override;
        [[nodiscard]] unsigned int get_vertical_margin() const override;
        [[nodiscard]] bool text_has_shadow() const override;

    public:
        using Render::Render;
    };
    class PlasticRender final : public Render {
    protected:
        [[nodiscard]] unsigned int get_height() const override;
        [[nodiscard]] Xml get_content() const override;
        [[nodiscard]] unsigned int get_vertical_margin() const override;
        [[nodiscard]] bool text_has_shadow() const override;

    public:
        using Render::Render;
    };

    class ForTheBadgeRender final : public Render {
    protected:
        [[nodiscard]] unsigned int get_height() const override;
        [[nodiscard]] Xml get_content() const override;
        [[nodiscard]] unsigned int get_vertical_margin() const override;
        [[nodiscard]] bool text_has_shadow() const override;
        unsigned int get_str_width(const std::optional<std::string> &str, TextType tt) const override;

        Xml render() override;

    public:
        using Render::Render;

    private:
        unsigned int label_text_min_x;
        unsigned int label_rect_width;
        unsigned int message_text_min_x;
        unsigned int message_rect_width;

        std::string up_label;
        std::string up_message;

        [[nodiscard]] Xml getLabelElement() const;
        [[nodiscard]] Xml getMessageElement() const;
    };

    class SocialRender final : public Render {
    protected:
        [[nodiscard]] unsigned int get_height() const override;
        [[nodiscard]] Xml get_content() const override;
        [[nodiscard]] unsigned int get_vertical_margin() const override;
        [[nodiscard]] bool text_has_shadow() const override;
        unsigned int get_str_width(const std::optional<std::string> &str, TextType tt) const override;

        Xml render() override;

    public:
        using Render::Render;

    private:
        unsigned int label_rect_width;
        unsigned int message_rect_width;

        [[nodiscard]] Xml getMessageBubble() const;
        [[nodiscard]] Xml getLabelText() const;
        [[nodiscard]] Xml getMessageText() const;
    };

}// namespace badge

#endif// BADGECPP_RENDER_IMPL_HPP_GUARD
