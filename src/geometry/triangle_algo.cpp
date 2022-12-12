#include "triangle_algo.h"

#include "polygon.h"
#include "triangle_group.h"
#include "math/integrate.h"


bool
algo::
isPointInsideTriangle(Triangle2 const & tri_1, Vector2 const & pt)
{
    std::array<scalar_t, 3> const coords
    {
        getBarycentricCoordinates(tri_1, pt)
    };
    bool const isInside
    {
        // if all barycentric coordinates 0 < c < 1 then it is inside
           (coords[0] > scalar_t(0)) && (coords[0] < scalar_t(1))
        && (coords[1] > scalar_t(0)) && (coords[1] < scalar_t(1))
        && (coords[2] > scalar_t(0)) && (coords[2] < scalar_t(1))
    };
    return isInside;
}


Vector2 const *
algo::
isOverlapping(Triangle2 const & tri_1, Triangle2 const & tri_2)
{
    for (int v_id{0}; v_id < 3; v_id++)
    {
        if (isPointInsideTriangle(tri_1, tri_2[v_id]))
        {
            return &(tri_2[v_id]);
        }
    }
    return nullptr;
}


Vector2 const *
algo::
areOverlapping(TriangleGroup const & group1, TriangleGroup const & group2)
{
    std::vector<Triangle2> const & triangles_1 {group1.getAbsTriangles()};
    std::vector<Triangle2> const & triangles_2 {group2.getAbsTriangles()};
    for (Triangle2 const & triangle_1 : triangles_1)
    {
        for (Triangle2 const & triangle_2 : triangles_2)
        {
            if (Vector2 const * pt = isOverlapping(triangle_1, triangle_2))
            {
                return pt;
            }
        }
    }
    return nullptr;
}


bool
algo::
isPointOnASegment(Segment2 const & s, Vector2 const & pt, scalar_t tol)
{
    Vector2 const direction{pt - s[0]};
    Vector2 const bdry_plane{s[1] - s[0]};
    Vector2 const normal{direction - dot(direction, bdry_plane)*bdry_plane};
    return magSqr(normal) < (tol*tol);
}


bool
algo::
isPointInsidePolygon(TriangleGroup const & g, Vector2 const & pt)
{
    for (auto const & t : g.getTriangles())
    {
        if (isPointInsideTriangle(t, pt)) return true;
    }
    return false;
}
