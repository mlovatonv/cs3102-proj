/// \file

#include <cstdlib>

#include "spatial/PRQuadTree.hpp"

// min lat 39
// max lat 43
// min lng -76
// max lng -70

/// Program entry point.
int main(void) {
    spatial::PRQuadTree t1(spatial::Rectangle(spatial::XY(-76, 39), spatial::XY(-70, 43)));

    double pu_lng, pu_lat, do_lng, do_lat;
    while (std::cin >> pu_lng >> pu_lat >> do_lng >> do_lat) {
        t1.insert(spatial::XY(pu_lng, pu_lat));
    }

    return EXIT_SUCCESS;
}
