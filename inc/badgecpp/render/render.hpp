#ifndef x1
#define x1
#include "badgecpp/badgecpp.hpp"
namespace badgecpp {
    class Render {
    protected:
        friend struct Badge;
        virtual void render(Badge &badge) = 0;
        virtual ~Render() {}
    };
}// namespace badgecpp
#endif