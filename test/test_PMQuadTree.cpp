#include "catch2/catch.hpp"
#include "spatial/PMQuadTree.hpp"

using namespace spatial;

TEST_CASE("spatial_join") {
    PMQuadTree t(Rectangle(XY(0, 0), XY(10, 10)));

    // t.insert(Line(XY(1, 1));

    REQUIRE(t.spatial_join(std::vector<XY>{XY(1, 1)}) == std::vector<int>{1});
}
