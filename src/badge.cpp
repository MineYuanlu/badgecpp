#include "badgecpp/badge.hpp"
#include "badgecpp/render.hpp"
namespace badge {
    std::string Badge::makeBadge() const {
        return Render::create(*this)->render().render();
    }
}// namespace badge