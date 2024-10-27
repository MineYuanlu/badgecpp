#include "badgecpp/badge.hpp"
#include "badgecpp/render.hpp"
namespace badge {
    std::string Badge::makeBadge() const {
        return makeBadgeXml().render();
    }
    Xml Badge::makeBadgeXml() const {
        return Render::create(*this)->render();
    }
}// namespace badge