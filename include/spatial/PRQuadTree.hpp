#ifndef CS3102_PROJ_SPATIAL_PRQUADTREE_HPP_
#define CS3102_PROJ_SPATIAL_PRQUADTREE_HPP_

#include "spatial/utils.hpp"

namespace spatial {

// https://stackoverflow.com/a/67346872
template <int NODE_CAPACITY = 1000>
class PRQuadTree {
   public:
    struct Node {
        Rectangle bbox;  // bounding box
        std::vector<std::shared_ptr<Node>> children;
        std::vector<XY> points;

        Node(const Rectangle& bbox) : bbox(bbox), children(0), points(0) {}

        bool is_full() const { return this->points.size() == NODE_CAPACITY; }

        bool is_leaf() const { return this->children.empty(); }

        void divide() {
            std::vector<Rectangle> quadrants = this->bbox.divide();

            for (auto const& q : quadrants) {
                std::shared_ptr<Node> new_child = std::make_shared<Node>(q);
                this->children.push_back(new_child);

                for (auto const& p : this->points) {
                    if (q.contains(p)) {
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
    PRQuadTree(const Rectangle& bbox) : root(std::make_shared<Node>(bbox)) {}

    PRQuadTree(const Rectangle& bbox, const std::vector<XY>& points)
        : root(std::make_shared<Node>(bbox)) {
        for (auto const& p : points) {
            this->insert(p);
        }
    }

    template <typename Shape>
    std::vector<XY> search(const Shape& shape) {
        std::vector<XY> result;

        std::function<void(std::shared_ptr<Node>)> dfs = [&](const std::shared_ptr<Node>& node) {
            for (auto const& p : node->points) {
                if (shape.contains(p)) {
                    result.push_back(p);
                }
            }

            for (auto const& c : node->children) {
                if (shape.intersects(c->bbox)) {
                    dfs(c);
                }
            }
        };

        dfs(this->root);

        return result;
    }

    void insert(const XY& point) {
        std::function<void(std::shared_ptr<Node>)> dfs = [&](const std::shared_ptr<Node>& node) {
            if (node->is_leaf()) {
                if (node->is_full()) {
                    node->divide();
                } else {
                    node->points.push_back(point);
                    return;
                }
            }

            for (auto const& c : node->children) {
                if (c->bbox.contains(point)) {
                    dfs(c);
                    return;
                }
            }
        };

        dfs(this->root);
    }
};

}  // namespace spatial

#endif  // CS3102_PROJ_SPATIAL_PRQUADTREE_HPP_