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
const int NO_RESULT = -1;

enum COLOR {
    BLACK,
    GREY,
    WHITE,
};

double mid(const double& a, const double& b);

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

double eudist(const XY& a, const XY& b);

struct Line {
    XY p1;
    XY p2;

    Line(const XY& p1, const XY& p2) : p1(p1), p2(p2) {}

    bool joins(const Line& o) const {
        // TO-DO
        return false;
    }
};

struct PolygonLine {
    Line l;  // line
    XY c;    // point inside polygon
    int id;  // polygon id

    bool contains(const XY& o) const {
        // TO-DO
        return false;
    }

    bool joins(const PolygonLine& o) const { return this->l.joins(o.l); }
};

struct Rectangle {
    XY bl;  // bottom left
    XY tr;  // top right

    Rectangle(const XY& bl, const XY& tr) : bl(bl), tr(tr) {}

    friend std::ostream& operator<<(std::ostream& out, const Rectangle& o) {
        out << "( " << o.bl << ", " << o.tr << " )";
        return out;
    }

    bool contains(const XY& o) const { return this->bl <= o && o <= this->tr; }

    bool intersects(const Rectangle& o) const {
        return this->bl.x <= o.tr.x && o.bl.x <= this->tr.x && this->bl.y <= o.tr.y &&
               o.bl.y <= this->tr.y;
    }

    bool intersects(const Line& o) const {
        // TO-DO
        return false;
    }

    bool intersects(const PolygonLine& pline) const { return this->intersects(pline.l); }

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

    bool intersects(const Rectangle& o) const {
        return this->bl.x <= o.tr.x && o.bl.x <= this->tr.x && this->bl.y <= o.tr.y &&
               o.bl.y <= this->tr.y;
    }
};

}  // namespace spatial

#endif  // CS3102_PROJ_SPATIAL_UTILS_HPP_