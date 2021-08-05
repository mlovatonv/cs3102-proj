#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch2/catch.hpp"
#include "spatial/PMQuadTree.hpp"

using namespace spatial;

TEST_CASE("test_spatial_join") {
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

TEST_CASE("benchmark_spatial_join") {
    auto generate = [](int n) {
        srand(time(NULL));
        std::vector<PolygonLine> plines;
        for (int i = 0; i < n; ++i) {
            double a = abs(rand()) % 100 + 1, b = abs(rand()) % 100 + 1, m = mid(a, b);
            plines.emplace_back(XY(a, a), XY(b, a), XY(m, m), i);
            plines.emplace_back(XY(b, a), XY(b, b), XY(m, m), i);
            plines.emplace_back(XY(b, b), XY(a, b), XY(m, m), i);
            plines.emplace_back(XY(a, b), XY(a, a), XY(m, m), i);
        }
        return plines;
    };

    for (int n = 10; n <= 100; n += 10) {
        BENCHMARK("build_pm_" + std::to_string(n)) {
            return PMQuadTree(Rectangle(XY(0, 0), XY(100, 100)), generate(n));
        };
    }

    for (int n = 10; n <= 100; n += 10) {
        PMQuadTree<1> t(Rectangle(XY(0, 0), XY(100, 100)), generate(n));
        BENCHMARK("spatial_join_" + std::to_string(n)) {
            return t.spatial_join(std::vector<XY>{XY(50, 50)});
        };
    }
}
