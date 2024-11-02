#ifndef BADGECPP_ICONS_HPP
#define BADGECPP_ICONS_HPP
#include "color.hpp"
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
namespace badge {
    struct Icon {
        std::string title;
        Color color;
        std::string_view icon;

        Icon(std::string title, Color color, std::string_view icon);
        Icon(std::string title, std::string color, const void *data, size_t size);

        [[nodiscard]] std::string get_uri() const;
    };
    /// @brief From Simple Icons
    /// https://github.com/simple-icons
    struct Icons {


        static const std::vector<Icon> &icons();

        static Icons &instance();


    private:
        Icons();
        std::vector<Icon> icons_;
    };
}// namespace badge
#endif// BADGECPP_ICONS_HPP