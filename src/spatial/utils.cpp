#include "spatial/utils.hpp"

namespace spatial {

double mid(const double& a, const double& b) { return (a + b) / 2.0; }

double eudist(const XY& a, const XY& b) { return std::hypot(a.x - b.x, a.y - b.y); }

double ccw(const XY& a, const XY& b, const XY& c) {
    return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
}

}  // namespace spatial
