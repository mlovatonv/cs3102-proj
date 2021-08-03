#include "catch2/catch.hpp"
#include "spatial/PMQuadTree.hpp"

using namespace spatial;

TEST_CASE("spatial_join") {
    PMQuadTree t(Rectangle(XY(0, 0), XY(10, 10)));

    // square 1
    t.insert(PolygonLine(XY(1, 1), XY(3, 1), XY(2, 2), 1));
    t.insert(PolygonLine(XY(3, 1), XY(3, 3), XY(2, 2), 1));
    t.insert(PolygonLine(XY(3, 3), XY(1, 3), XY(2, 2), 1));
    t.insert(PolygonLine(XY(1, 3), XY(1, 1), XY(2, 2), 1));

    // square 2
    t.insert(PolygonLine(XY(3, 3), XY(5, 3), XY(4, 4), 2));
    t.insert(PolygonLine(XY(5, 3), XY(5, 5), XY(4, 4), 2));
    t.insert(PolygonLine(XY(5, 5), XY(3, 5), XY(4, 4), 2));
    t.insert(PolygonLine(XY(3, 5), XY(3, 3), XY(4, 4), 2));

    REQUIRE(t.spatial_join(std::vector<XY>{XY(0, 0)}) == std::vector<int>{-1});
    REQUIRE(t.spatial_join(std::vector<XY>{XY(2.5, 2.5)}) == std::vector<int>{1});
    REQUIRE(t.spatial_join(std::vector<XY>{XY(4.5, 4.5)}) == std::vector<int>{2});
}
