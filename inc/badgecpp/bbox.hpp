#ifndef BBOX_HPP
#define BBOX_HPP
namespace badge {
    struct BBox {
        int x, y, w, h;
        constexpr BBox(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

        [[nodiscard]] constexpr bool contains(int px, int py) const {
            return px >= x && px < x + w && py >= y && py < y + h;
        }
        [[nodiscard]] constexpr int area() const {
            return w * h;
        }
    };
}// namespace badge
#endif