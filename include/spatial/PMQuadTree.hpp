#ifndef CS3102_PROJ_SPATIAL_PMQUADTREE_HPP_
#define CS3102_PROJ_SPATIAL_PMQUADTREE_HPP_

#include "spatial/utils.hpp"

namespace spatial {

// PM3 PM Quadtree
template <int NODE_CAPACITY = 1>
class PMQuadTree {
   public:
    struct Node {
        Rectangle bbox;  // bounding box
        std::vector<std::shared_ptr<Node>> children;
        std::vector<XY> points;
        std::vector<PolygonLine> plines;

        Node(const Rectangle& bbox) : bbox(bbox), children(0), points(0) {}

        // 2 cases
        // 1. one pline
        // 2. two plines joined
        bool is_full() const { return this->points.size() == NODE_CAPACITY; }

        bool is_leaf() const { return this->children.empty(); }

        bool is_empty() const { return this->plines.empty(); }

        int locate(const XY& point) const {
            int result = NO_RESULT;

            // TO-DO: See if this works
            for (auto const& pl : this->plines) {
                if (pl.contains(point)) {
                    result = pl.id;
                    break;
                }
            }

            return result;
        }

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

                for (auto const& pl : this->plines) {
                    if (q.intersects(pl)) {
                        new_child->plines.push_back(pl);
                    }
                }
            }

            this->points.clear();
            this->plines.clear();
        }
    };

   private:
    std::shared_ptr<Node> root;

   public:
    PMQuadTree(const Rectangle& bbox) : root(std::make_shared<Node>(bbox)) {}

    PMQuadTree(const Rectangle& bbox, const std::vector<PolygonLine>& plines)
        : root(std::make_shared<Node>(bbox)) {
        for (auto const& pl : plines) {
            this->insert(pl);
        }
    }

    void insert(const PolygonLine& pline) {
        std::function<void(std::shared_ptr<Node>)> dfs = [&](const std::shared_ptr<Node>& node) {
            if (node->is_leaf()) {
                node->plines.push_back(pline);

                for (auto const& p : {pline.l.p1, pline.l.p2}) {
                    if (node->bbox.contains(p)) {
                        if (node->is_full()) {
                            if (node->points[0] == p) {
                                return;  // join
                            } else {
                                node->divide();
                            }

                        } else {
                            node->points.push_back(p);
                            return;
                        }
                    }
                }
            }

            for (auto const& c : node->children) {
                if (c->bbox.intersects(pline)) {
                    dfs(c);
                    return;
                }
            }
        };

        dfs(this->root);
    }

    int locate(const XY& point) {
        int result = NO_RESULT;

        std::function<void(std::shared_ptr<Node>)> dfs = [&](const std::shared_ptr<Node>& node) {
            for (auto const& c : node->children) {
                if (c->bbox.contains(point)) {
                    // TO-DO: Refactor
                    if (c->is_leaf()) {
                        if (c->is_empty()) {
                            // check brothers
                            for (auto const& b : node->children) {
                                result = b->locate(point);
                                if (result != NO_RESULT) return;
                            }
                        } else {
                            result = c->locate(point);
                        }
                        if (result != NO_RESULT) return;
                    } else {
                        dfs(c);
                        break;
                    }
                }
            }
        };

        dfs(this->root);

        return result;
    }

    std::vector<int> spatial_join(const std::vector<XY>& points) {
        std::vector<int> result;
        result.reserve(points.size());

        for (auto const& p : points) {
            result.push_back(this->locate(p));
        }

        return result;
    }
};

}  // namespace spatial

#endif  // CS3102_PROJ_SPATIAL_PMQUADTREE_HPP_