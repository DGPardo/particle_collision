#ifndef BOUNDARIES_MANAGER_H_
#define BOUNDARIES_MANAGER_H_


#include "polygon.h"
#include "segment.h"
#include "triangle_group.h"
#include <vector>
#include <memory>


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
    TriangleGroup const & getDomain() const;

private:
    BoundariesManager() = default;
    std::unique_ptr<TriangleGroup> _group;
    std::vector<Segment2> _boundary;
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
}


#endif


