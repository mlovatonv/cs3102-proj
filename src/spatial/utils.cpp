#include "spatial/utils.hpp"

namespace spatial {

double mid(const double& a, const double& b) { return (a + b) / 2.0; }

double eudist(const XY& a, const XY& b) { return std::hypot(a.x - b.x, a.y - b.y); }

}  // namespace spatial
