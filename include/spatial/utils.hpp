#ifndef CS3102_PROJ_SPATIAL_UTILS_HPP_
#define CS3102_PROJ_SPATIAL_UTILS_HPP_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <limits>
#include <memory>
#include <ostream>
#include <vector>

#include "dbg/dbg.h"

namespace spatial {

const double INF = std::numeric_limits<double>::max();
const double U = std::numeric_limits<double>::min();

enum COLOR {
    BLACK,
    GREY,
    WHITE,
};

double mid(const double& a, const double& b) { return (a + b) / 2.0; }

struct XY {
    double x;
    double y;

    XY() : x(INF), y(INF) {}

    XY(const double& x, const double& y) : x(x), y(y) {}

    bool operator<=(const XY& o) const { return this->x <= o.x && this->y <= o.y; }

    bool operator>=(const XY& o) const { return this->x >= o.x && this->y >= o.y; }

    bool operator==(const XY& o) const { return this->x == o.x && this->y == o.y; }

    friend std::ostream& operator<<(std::ostream& out, const XY& o) {
        out << "( " << o.x << ", " << o.y << " )";
        return out;
    }
};

double eudist(const XY& a, const XY& b) { return std::hypot(a.x - b.x, a.y - b.y); }

struct Rectangle {
    XY bl;  // bottom left
    XY tr;  // top right

    Rectangle(const XY& bl, const XY& tr) : bl(bl), tr(tr) {}

    friend std::ostream& operator<<(std::ostream& out, const Rectangle& o) {
        out << "( " << o.bl << ", " << o.tr << " )";
        return out;
    }

    bool contains(const XY& o) const { return this->bl <= o && o <= this->tr; }

    bool intersects(const Rectangle& o) const { return o.bl <= this->bl || this->tr <= o.tr; }

    std::vector<Rectangle> divide() const {
        XY c = XY(mid(bl.x, tr.x), mid(bl.y, tr.y));
        return std::vector<Rectangle>{Rectangle(XY(bl.x, c.y + U), XY(c.x, tr.y)),
                                      Rectangle(XY(c.x + U, c.y + U), tr), Rectangle(bl, c),
                                      Rectangle(XY(c.x + U, bl.x), XY(tr.x, c.y))};
    }
};

struct Circle {
    XY bl;     // bottom left
    XY tr;     // top right
    XY c;      // center
    double d;  // distance

    Circle(const XY& c, const double& d) : bl(c.x - d, c.y - d), tr(c.x + d, c.y + d), c(c), d(d) {}

    friend std::ostream& operator<<(std::ostream& out, const Circle& o) {
        out << "( " << o.bl << ", " << o.tr << ", " << o.c << ", " << o.d << " )";
        return out;
    }

    bool contains(const XY& o) const { return this->bl <= o && o <= this->tr && eudist(c, o) <= d; }

    bool intersects(const Rectangle& o) const { return o.bl <= this->tr || this->bl <= o.tr; }
};

}  // namespace spatial

#endif  // CS3102_PROJ_SPATIAL_UTILS_HPP_