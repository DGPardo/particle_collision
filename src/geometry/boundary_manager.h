#ifndef BOUNDARIES_MANAGER_H_
#define BOUNDARIES_MANAGER_H_


#include "bounding_box.h"
#include "polygon.h"
#include "segment.h"
#include "triangle_group.h"

#include <vector>
#include <memory>


/*----------------------------------------------------------------------------*\
Name:   BoundariesManager
Intent: 
    This class contains one and only boundary that will be used to limit the 
    position of the different particles colliding with each other

Limitations:
    Boundary can only be 2D. Boundary is only implemented via collection
    of Segments
\*----------------------------------------------------------------------------*/


class BoundariesManager
{
public:
    static BoundariesManager & getSingleton();

    template<label_t n>
    void setBoundary
    (
        ConvexPolygon<n> const & polygon
    );

    std::vector<Segment2> const & getBoundary() const;
    Rectangle const & getBoundingBox() const;
    TriangleGroup const & getDomain() const;

private:
    BoundariesManager() = default;
    BoundariesManager(BoundariesManager const & other) = delete;
    BoundariesManager(BoundariesManager&& other) = delete;
    std::unique_ptr<TriangleGroup> _group;
    std::vector<Segment2> _boundary;
    std::unique_ptr<Rectangle> _bbox;
};


//------------------------------------------------------------------------------
//- Template function Implementation


template<label_t n>
void
BoundariesManager::
setBoundary
(
    ConvexPolygon<n> const & polygon
)
{
    _group = std::make_unique<TriangleGroup>
    (
        Vector2{0, 0},
        Vector2{0, 0}
    );
    for (Triangle2 const & t : polygon.triangles)
    {
        _group->addTriangle(t);
    }

    _boundary.clear();
    _boundary.resize(n);

    for (label_t v_id{0}; v_id != (n - 1); ++v_id)
    {
        _boundary[v_id] = {polygon.vertices[v_id], polygon.vertices[v_id + 1]};
    }
    _boundary[n-1] = {polygon.vertices[n-1], polygon.vertices[0]};

    _bbox = std::make_unique<Rectangle>(algo::getBoundingBox(_boundary));
}


#endif

