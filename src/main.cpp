/// \file

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include "spatial/PMQuadTree.hpp"
#include "spatial/PRQuadTree.hpp"

using namespace spatial;

/// Program entry point.
int main(void) {
    Rectangle points_bbox = Rectangle(XY(-76, 39), XY(-70, 43));
    Rectangle polygons_bbox = Rectangle(XY(-75, 40), XY(-73, 41));

    std::vector<XY> pick_up_points, drop_off_points;
    double pu_lng, pu_lat, do_lng, do_lat;
    while (std::cin >> pu_lng >> pu_lat >> do_lng >> do_lat) {
        pick_up_points.emplace_back(pu_lng, pu_lat);
        drop_off_points.emplace_back(pu_lng, pu_lat);
    }

    std::vector<PolygonLine> polygon_lines;
    double p1_lng, p1_lat, p2_lng, p2_lat, c_lng, c_lat;
    int id;
    while (std::cin >> p1_lng >> p1_lat >> p2_lng >> p2_lat >> c_lng >> c_lat >> id) {
        polygon_lines.emplace_back(XY(p1_lng, p1_lat), XY(p2_lng, p2_lat), XY(c_lng, c_lat), id);
    }

    std::function<void(void)> query1 = [&]() {
        // ¿Qué viajes tuvieron como origen y destino el mismo barrio?

        // process
        PMQuadTree t(polygons_bbox, polygon_lines);
        std::vector<int> pick_up_result = t.spatial_join(pick_up_points);
        std::vector<int> drop_off_result = t.spatial_join(drop_off_points);

        // result
        int result = 0;
        for (int i = 0; i < pick_up_result.size(); ++i) {
            result += pick_up_result[i] == drop_off_result[i];
        }
        std::cout << result << std::endl;
    };

    std::function<void(void)> query2 = [&]() {
        // ¿Cuáles son los barrios con mayor cantidad de viajes? Retornar los top 5. (Considerando
        // apenas las coordenadas de partida)

        // input
        const int N = 5;

        // process
        PMQuadTree t(polygons_bbox, polygon_lines);
        std::vector<int> pick_up_result = t.spatial_join(pick_up_points);

        // result
        std::map<int, int> freq;
        std::vector<std::pair<int, int>> v;
        for (auto const& i : pick_up_result) ++freq[i];
        for (auto const& [key, val] : freq) v.emplace_back(val, key);
        std::sort(v.begin(), v.end(), std::greater<std::pair<int, int>>());
        for (int i = 0; i < N; ++i) {
            std::cout << v[i].second << std::endl;
        }
    };

    std::function<void(void)> query3 = [&]() {
        // Dados dos puntos P1 y P2 que representan una region rectangular, retornar el número de
        // viajes que comenzaron en tal región

        // input
        XY p1 = XY(-76, 39);
        XY p2 = XY(-74, 41);

        // process
        PRQuadTree t(points_bbox, pick_up_points);
        std::vector<XY> result = t.search<Rectangle>(Rectangle(p1, p2));

        // output
        std::cout << result.size() << " points" << std::endl;
        for (auto& p : result) {
            std::cout << p << std::endl;
        }
    };

    std::function<void(void)> query5 = [&]() {
        // Dado un punto P y una distancia D, encontrar los viajes que empezaron o terminaron a una
        // distancia D como máximo a partir del punto P.

        // input
        XY p = XY(-74, 41);
        double d = 1.54;

        // consider all points
        std::vector<XY> all_points(pick_up_points);
        all_points.insert(all_points.end(), drop_off_points.begin(), drop_off_points.end());

        // process
        PRQuadTree t(points_bbox, all_points);
        std::vector<XY> result = t.search<Circle>(Circle(p, d));

        // output
        std::cout << result.size() << " points" << std::endl;
        for (auto& p : result) {
            std::cout << p << std::endl;
        }
    };

    return EXIT_SUCCESS;
}
