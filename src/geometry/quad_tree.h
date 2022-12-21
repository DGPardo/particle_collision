#ifndef QUAD_TREE_H_
#define QUAD_TREE_H_


#include "geometry/polygon.h"
#include "math/vector_math.h"

#include <memory>
#include <vector>


struct QuadTreeNode
{
    Vector2 position;
    void* data;

    template<class T>
    static constexpr QuadTreeNode make(Vector2 position, T * data)
    {
        return QuadTreeNode(std::move(position), static_cast<void*>(data));
    }
};


class QuadTree
{
public:
    // Boundary must have edges parallel to either horizontal or vertical axis
    QuadTree(Rectangle boundary, label_t capacity=4);

    bool insert(QuadTreeNode node);

    [[nodiscard]] std::vector<QuadTreeNode> query(Rectangle const & lookup_region) const;

protected:
    void subdivide();

    Rectangle const _boundary;
    label_t const _capacity;
    std::vector<QuadTreeNode> _points;
    bool _is_subdivided;

    std::unique_ptr<QuadTree> _north_east;
    std::unique_ptr<QuadTree> _north_west;
    std::unique_ptr<QuadTree> _south_west;
    std::unique_ptr<QuadTree> _south_east;
};


#endif

