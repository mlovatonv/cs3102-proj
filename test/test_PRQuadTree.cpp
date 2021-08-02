#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch2/catch.hpp"
#include "spatial/PRQuadTree.hpp"

using namespace spatial;

TEST_CASE("search rectangle") {
    PRQuadTree t(Rectangle(XY(0, 0), XY(10, 10)));

    t.insert(XY(1, 1));
    t.insert(XY(3, 3));
    t.insert(XY(5, 5));

    REQUIRE(t.search<Rectangle>(Rectangle(XY(0, 0), XY(2, 2))) == std::vector<XY>{XY(1, 1)});
    REQUIRE(t.search<Rectangle>(Rectangle(XY(2, 2), XY(4, 4))) == std::vector<XY>{XY(3, 3)});
    REQUIRE(t.search<Rectangle>(Rectangle(XY(3, 3), XY(5, 5))) ==
            std::vector<XY>{XY(3, 3), XY(5, 5)});
}

TEST_CASE("search circle") {
    PRQuadTree t(Rectangle(XY(0, 0), XY(10, 10)));

    t.insert(XY(1, 1));
    t.insert(XY(3, 3));
    t.insert(XY(5, 5));

    REQUIRE(t.search<Circle>(Circle(XY(1, 1), 1)) == std::vector<XY>{XY(1, 1)});
    REQUIRE(t.search<Circle>(Circle(XY(3, 3), 2)) == std::vector<XY>{XY(3, 3)});
    REQUIRE(t.search<Circle>(Circle(XY(3, 3), 3)) == std::vector<XY>{XY(1, 1), XY(3, 3), XY(5, 5)});
}
