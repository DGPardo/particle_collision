#ifndef TRIANGLE_ALGO_H
#define TRIANGLE_ALGO_H


#include "math/vector_math.h"
#include "math/integrate.h"
#include "segment.h"
#include "triangle.h"


#include <limits>
#include <memory>
#include <vector>


template<label_t NVerts> struct ConvexPolygon;
class TriangleGroup;


namespace algo
{

constexpr scalar_t
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

//------------------------------------------------------------------------------

constexpr scalar_t
triangleAreaMoment(Triangle2 const & verts, scalar_t const area, Vector2 const & pt)
{
    //- Moment around axis perpendicular to the plane of the triangle
    //- passing through the centroid
    auto r_sq = [&verts](scalar_t l1, scalar_t l2)
    {
        //- l1 is fist barycentric coordinate
        //- l2 is the second barycentric coordinate
        //- centroid is at l1=l2=l3=1/3

        std::array<scalar_t, 2> delta_r
        {
            (l1-1.f/3.f)*verts[0][0] + (l2-1.f/3.f)*verts[1][0] + (2.f/3.f-l1-l2)*verts[2][0],
            (l1-1.f/3.f)*verts[0][1] + (l2-1.f/3.f)*verts[1][1] + (2.f/3.f-l1-l2)*verts[2][1]
        };
        return delta_r[0]*delta_r[0] + delta_r[1]*delta_r[1];
    };

    auto integrate = [&]()
    {
        //- Integral of r**2 dA
        auto outer = [&](scalar_t l2)
        {
            auto inner = [&](scalar_t l1)
            {
                return r_sq(l1, l2);
            };
            return algo::integral1(inner, 0, 1-l2);
        };
        return algo::integral1(outer, 0, 1);
    };

    //- Moment Around centroid:
    scalar_t const area_moment {2 * area * integrate()};

    //- Parallel axis theorem ==> Move moment of inertia
    Vector2 centroid {1.f/3.f * (verts[0] + verts[1] + verts[2])};
    return area_moment + area*magSqr(pt - centroid);
}


//------------------------------------------------------------------------------

constexpr std::array<scalar_t, 3>
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

    constexpr scalar_t tolerance = std::numeric_limits<scalar_t>::epsilon()*100;
    detT = (detT > 0) ? (detT + tolerance) : (detT - tolerance);

    std::array<scalar_t, 3> coords;
    coords[0] = ((v1[1] - v2[1])*(pt[0] - v2[0]) + (v2[0] - v1[0])*(pt[1] - v2[1])) / detT;
    coords[1] = ((v2[1] - v0[1])*(pt[0] - v2[0]) + (v0[0] - v2[0])*(pt[1] - v2[1])) / detT;
    coords[2] = 1 - coords[0] - coords[1];
    return coords;
}

//------------------------------------------------------------------------------

bool
isPointInsideTriangle(Triangle2 const & tri_1, Vector2 const & pt);


//------------------------------------------------------------------------------

bool
isPointInsidePolygon(TriangleGroup const & g, Vector2 const & pt);

//------------------------------------------------------------------------------

std::unique_ptr<Vector2>
isOverlapping(Triangle2 const & tri_1, Triangle2 const & tri_2);


std::unique_ptr<Vector2>
areOverlapping(TriangleGroup const & group1, TriangleGroup const & group2);

//------------------------------------------------------------------------------

scalar_t
isPointOnASegment(Segment2 const & s, Vector2 const & pt);

}


#endif
