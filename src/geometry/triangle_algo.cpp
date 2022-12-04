#include "triangle_algo.h"

#include "polygon.h"
#include "triangle_group.h"

#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*100)


scalar_t
algo::
triangleArea(Triangle2 const & verts)
{
    // 2D Only!!!
    //- Formula from: https://sciencing.com/area-triangle-its-vertices-8489292.html
    scalar_t const half{0.5};
    return half *
    (
          verts[0][0]                  // Ax
        * (verts[1][1] - verts[2][1])  // * (By - Cy)
        + verts[1][0]                  // + Bx
        * (verts[2][1] - verts[0][1])  // * (Cy - Ay)
        + verts[2][0]                  // + Cx
        * (verts[0][1] - verts[1][1])  // * (Ay - By)
    );
}


std::array<scalar_t, 3>
algo::
getBarycentricCoordinates(Triangle2 const & vertices, Vector2 const & pt)
{
    //- https://en.wikipedia.org/wiki/Barycentric_coordinate_system

    Vector2 const & v0{vertices[0]};
    Vector2 const & v1{vertices[1]};
    Vector2 const & v2{vertices[2]};

    scalar_t detT
    {
        (v1[1] - v2[1])*(v0[0] - v2[0])
      + (v2[0] - v1[0])*(v0[1] - v2[1])
    };
    detT = (detT > 0) ? (detT + TOL) : (detT - TOL);

    std::array<scalar_t, 3> coords;
    coords[0] = ((v1[1] - v2[1])*(pt[0] - v2[0]) + (v2[0] - v1[0])*(pt[1] - v2[1])) / detT;
    coords[1] = ((v2[1] - v0[1])*(pt[0] - v2[0]) + (v0[0] - v2[0])*(pt[1] - v2[1])) / detT;
    coords[2] = 1 - coords[0] - coords[1];
    return coords;
}


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


bool
algo::
isOverlapping(Triangle2 const & tri_1, Triangle2 const & tri_2)
{
    for (int v_id{0}; v_id < 3; v_id++)
    {
        if (isPointInsideTriangle(tri_1, tri_2[v_id]))
        {
            return true;
        }
    }
    return false;
}


bool
algo::
areOverlapping(TriangleGroup const & group1, TriangleGroup const & group2)
{
    std::vector<Triangle2> const & triangles_1 {group1.getTriangles()};
    std::vector<Triangle2> const & triangles_2 {group2.getTriangles()};
    for (Triangle2 const & triangle_1 : triangles_1)
    {
        Triangle2 const tri1_abs_position{triangle_1 + group1.position};
        for (Triangle2 const & triangle_2 : triangles_2)
        {
            if (isOverlapping(tri1_abs_position, triangle_2 + group2.position))
            {
                return true;
            }
        }
    }
    return false;
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
