#ifndef CS3102_PROJ_SPATIAL_PRQUADTREE_HPP_
#define CS3102_PROJ_SPATIAL_PRQUADTREE_HPP_

#include "spatial/utils.hpp"

namespace spatial {

// https://stackoverflow.com/questions/67325769/how-to-determine-the-optimal-capacity-for-quadtree-subdivision
template <int NODE_CAPACITY = 1000>
class PRQuadTree {
   public:
    struct Node {
        Rectangle bbox;  // bounding box
        std::vector<std::shared_ptr<Node>> children;
        std::vector<XY> points;

        Node(const Rectangle& bbox) : bbox(bbox), children(0), points(0) {}

        Node(const Rectangle& bbox, const std::vector<XY>& points)
            : bbox(bbox), children(0), points(points) {}

        COLOR color() const {
            if (this->points.size() == 0) return WHITE;
            if (this->points.size() == NODE_CAPACITY) return BLACK;
            return GREY;
        };

        bool is_leaf() const { return this->children.empty(); }
    };

   private:
    std::shared_ptr<Node> root;

    std::shared_ptr<Node> _search(const std::shared_ptr<Node>& node, const XY& point) {
        if (node->is_leaf()) {
            return node;
        } else {
            std::function<bool(std::shared_ptr<Node>)> contains_point =
                [&](const std::shared_ptr<Node>& x) { return x->bbox.contains(point); };

            auto it = std::find_if(node->children.begin(), node->children.end(), contains_point);

            assert(it != node->children.end());

            return this->_search(*it, point);
        }
    }

   public:
    PRQuadTree(const Rectangle& bbox) { this->root = std::make_shared<Node>(bbox); }

    std::shared_ptr<Node> search(const XY& point) { return this->_search(this->root, point); }

    void insert(const XY& point) {
        std::shared_ptr<Node> cur = this->root;

        while (true) {
            std::shared_ptr<Node> node = this->_search(cur, point);

            if (node->color() == BLACK) {
                std::vector<Rectangle> rectangles = node->bbox.divide();

                std::function<void(Rectangle)> divide_points = [&](const Rectangle& x) {
                    std::shared_ptr<Node> new_node = std::make_shared<Node>(x);
                    node->children.push_back(new_node);
                    std::function<void(XY)> inside_node = [&](const XY& x) {
                        if (new_node->bbox.contains(x)) {
                            new_node->points.push_back(x);
                        }
                    };
                    std::for_each(node->points.begin(), node->points.end(), inside_node);
                };

                std::for_each(rectangles.begin(), rectangles.end(), divide_points);

                node->points.clear();

                cur = node;
            } else {
                node->points.push_back(point);
                break;
            }
        }
    }
};

}  // namespace spatial

#endif  // CS3102_PROJ_SPATIAL_PRQUADTREE_HPP_