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

double ccw(const XY& a, const XY& b, const XY& c);

struct Line {
    XY p1;
    XY p2;
    double m;

    Line(const XY& p1, const XY& p2) : p1(p1), p2(p2) {}

    bool intersects(const Line& o) const {
        // no collinear points
        return (ccw(this->p1, o.p1, o.p2) != ccw(this->p2, o.p1, o.p2) &&
                ccw(this->p1, this->p2, o.p1) != ccw(this->p1, this->p2, o.p2));
    }
};

struct PolygonLine {
    Line l;  // line
    XY c;    // point inside polygon
    int id;  // polygon id

    PolygonLine(const XY& p1, const XY& p2, const XY& c, int id) : l(p1, p2), c(c), id(id) {}

    bool contains(const XY& o) const {
        // let's assume that the point cannot be over the line
        // returns if c and o are in the same side of the line
        // https://stackoverflow.com/a/28555585
        return ((l.p1.y - l.p2.y) * (c.x - l.p1.x) +
                (l.p2.x - l.p1.x) * (c.y - l.p1.y) * (l.p1.y - l.p2.y) * (o.x - l.p1.x) +
                (l.p2.x - l.p1.x) * (o.y - l.p1.y)) > 0;
    }
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
        XY br(tr.x, bl.y), tl(bl.x, tl.y);
        for (auto const& l : {Line(bl, br), Line(br, tr), Line(tr, tl), Line(tl, bl)}) {
            if (l.intersects(o)) {
                return true;
            }
        }
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