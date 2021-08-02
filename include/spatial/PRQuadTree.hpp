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

        bool is_full() const { return this->points.size() == NODE_CAPACITY; };

        bool is_leaf() const { return this->children.empty(); }

        void divide() {
            std::vector<Rectangle> rectangles = this->bbox.divide();

            for (auto& r : rectangles) {
                std::shared_ptr<Node> new_child = std::make_shared<Node>(r);
                this->children.push_back(new_child);

                for (auto& p : this->points) {
                    if (r.contains(p)) {
                        new_child->points.push_back(p);
                    }
                }
            }

            this->points.clear();
        }
    };

   private:
    std::shared_ptr<Node> root;

   public:
    PRQuadTree(const Rectangle& bbox) { this->root = std::make_shared<Node>(bbox); }

    template <typename Shape>
    std::vector<XY> search(const Shape& shape) {
        std::vector<XY> result;

        std::function<void(std::shared_ptr<Node>)> dfs = [&](const std::shared_ptr<Node>& node) {
            for (auto& p : node->points) {
                if (shape.contains(p)) {
                    result.push_back(p);
                }
            }

            for (auto& c : node->children) {
                if (shape.intersects(c->bbox)) {
                    dfs(c);
                }
            }
        };

        dfs(this->root);

        return result;
    }

    void insert(const XY& point) {
        std::function<bool(std::shared_ptr<Node>)> contains_point =
            [&](const std::shared_ptr<Node>& x) { return x->bbox.contains(point); };

        std::function<void(std::shared_ptr<Node>)> dfs = [&](const std::shared_ptr<Node>& node) {
            if (node->is_leaf()) {
                if (node->is_full()) {
                    node->divide();
                } else {
                    node->points.push_back(point);
                    return;
                }
            }
            auto it = std::find_if(node->children.begin(), node->children.end(), contains_point);

            assert(it != node->children.end());

            dfs(*it);
        };

        dfs(this->root);
    }
};

}  // namespace spatial

#endif  // CS3102_PROJ_SPATIAL_PRQUADTREE_HPP_