#ifndef QUAD_TREE_H_
#define QUAD_TREE_H_


#include "geometry/polygon.h"
#include "math/vector_math.h"

#include <memory>
#include <vector>


/*----------------------------------------------------------------------------*\
Name:   QuadTreeNode
Intent: 
    Represents a given data point in space and the associated data with it

Limitations:
    void pointers are to be treated carefully
    
\*----------------------------------------------------------------------------*/


struct QuadTreeNode
{
    Vector2 position;
    void * data;

    template<class T>
    static constexpr QuadTreeNode make(Vector2 position, T * data)
    {
        return QuadTreeNode(std::move(position), static_cast<void *>(data));
    }
};


/*----------------------------------------------------------------------------*\
Name:   QuadTree
Intent: 
    This object allows to sort in 2D space data. The capacity of the quad tree
    represent how many points can live within the same "Rectangle"
    
    Time Complexity:
        - Insertion O(log(N))
        - Query O(log(N))
    
    Space Complexity
        - storage O(N)

Limitations:
    2-Dimensional space sorting
    
\*----------------------------------------------------------------------------*/


class QuadTree
{
public:
    // Boundary must have edges parallel to either horizontal or vertical axis
    QuadTree(Rectangle boundary, label_t capacity=2);

    bool insert(QuadTreeNode const & node);

    [[nodiscard]] std::vector<QuadTreeNode> query(Rectangle const & lookup_region) const;

    Rectangle const boundary;
    label_t const capacity;
    bool is_subdivided;
    std::unique_ptr<QuadTree> north_east;
    std::unique_ptr<QuadTree> north_west;
    std::unique_ptr<QuadTree> south_west;
    std::unique_ptr<QuadTree> south_east;

protected:
    void subdivide();
    std::vector<QuadTreeNode> _points;
};


#endif
