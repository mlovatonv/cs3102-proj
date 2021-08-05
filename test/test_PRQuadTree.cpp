#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch2/catch.hpp"
#include "spatial/PRQuadTree.hpp"

using namespace spatial;

TEST_CASE("test_search_rectangle") {
    PRQuadTree<1> t(Rectangle(XY(0, 0), XY(10, 10)));

    t.insert(XY(1, 1));
    t.insert(XY(3, 3));
    t.insert(XY(5, 5));

    REQUIRE(t.search<Rectangle>(Rectangle(XY(0, 0), XY(2, 2))) == std::vector<XY>{XY(1, 1)});
    REQUIRE(t.search<Rectangle>(Rectangle(XY(2, 2), XY(4, 4))) == std::vector<XY>{XY(3, 3)});
    REQUIRE(t.search<Rectangle>(Rectangle(XY(3, 3), XY(5, 5))) ==
            std::vector<XY>{XY(5, 5), XY(3, 3)});
}

TEST_CASE("test_search_circle") {
    PRQuadTree<1> t(Rectangle(XY(0, 0), XY(10, 10)));

    t.insert(XY(1, 1));
    t.insert(XY(3, 3));
    t.insert(XY(5, 5));

    REQUIRE(t.search<Circle>(Circle(XY(1, 1), 1)) == std::vector<XY>{XY(1, 1)});
    REQUIRE(t.search<Circle>(Circle(XY(3, 3), 2)) == std::vector<XY>{XY(3, 3)});
    REQUIRE(t.search<Circle>(Circle(XY(3, 3), 3)) == std::vector<XY>{XY(5, 5), XY(3, 3), XY(1, 1)});
}

TEST_CASE("benchmark_search_rectangle") {
    auto generate = [](int n) {
        srand(time(NULL));
        std::vector<XY> points;
        for (int i = 0; i < n; ++i)
            points.emplace_back(abs(rand()) % 100 + 1, abs(rand()) % 100 + 1);
        return points;
    };

    for (int n = 10; n <= 100; n += 10) {
        BENCHMARK("build_pr_" + std::to_string(n)) {
            return PRQuadTree<1>(Rectangle(XY(0, 0), XY(100, 100)), generate(n));
        };
    }

    for (int n = 10; n <= 100; n += 10) {
        PRQuadTree<1> t(Rectangle(XY(0, 0), XY(100, 100)), generate(n));
        BENCHMARK("search_rectangle_" + std::to_string(n)) {
            return t.search<Rectangle>(Rectangle(XY(5, 5), XY(50, 50)));
        };
    }
}

TEST_CASE("benchmark_search_circle") {
    auto generate = [](int n) {
        srand(time(NULL));
        std::vector<XY> points;
        for (int i = 0; i < n; ++i)
            points.emplace_back(abs(rand()) % 100 + 1, abs(rand()) % 100 + 1);
        return points;
    };

    for (int n = 10; n <= 100; n += 10) {
        PRQuadTree<1> t(Rectangle(XY(0, 0), XY(100, 100)), generate(n));
        BENCHMARK("search_circle_" + std::to_string(n)) {
            return t.search<Circle>(Circle(XY(50, 50), 30));
        };
    }
}