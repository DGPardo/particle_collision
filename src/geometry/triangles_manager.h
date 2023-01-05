#ifndef TRIANGLES_MANAGER_H_
#define TRIANGLES_MANAGER_H_


#include "triangle.h"
#include "triangle_group.h"
#include "segment.h"
#include <vector>


/*----------------------------------------------------------------------------*\
Name:   TrianglesManager
Intent: 
    Owns and tracks all particles and their states. It is used to add or remove
    particles from the simulation.

Limitations:
    Misuse of a Singleton pattern may cause undesired behaviour
    
\*----------------------------------------------------------------------------*/


class TrianglesManager
{
public:
    static TrianglesManager & getSingleton();

    template<label_t N>
    void addGroup
    (
        Vector2 position,
        Vector2 velocity,
        ConvexPolygon<N> const & polygon
    );

    std::vector<TriangleGroup> & getTriangleGroups();

private:
    TrianglesManager() = default;
    std::vector<TriangleGroup> _triangleGroups;
};


//------------------------------------------------------------------------------
//- Template function Implementation


template<label_t N>
void
TrianglesManager::
addGroup
(
    Vector2 position,
    Vector2 velocity,
    ConvexPolygon<N> const & polygon
)
{
    TriangleGroup & group
    {
        _triangleGroups.emplace_back
        (
            std::move(position),
            std::move(velocity)
        )
    };

    for (auto const & triangle : polygon.triangles)
    {
        group.addTriangle(triangle);
    }

    for (label_t v_id{0}; v_id != (N - 1); ++v_id)
    {
        Segment2 bdry {polygon.vertices[v_id], polygon.vertices[v_id + 1]};
        group.addBoundarySegment(std::move(bdry));
    }
    Segment2 bdry {polygon.vertices[N-1], polygon.vertices[0]};
    group.addBoundarySegment(std::move(bdry));
}


#endif

