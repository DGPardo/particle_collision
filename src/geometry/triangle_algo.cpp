#include "triangle_algo.h"

#include "bounding_box.h"
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
           (coords[0] > scalar_t(0.f)) && (coords[0] < scalar_t(1.f))
        && (coords[1] > scalar_t(0.f)) && (coords[1] < scalar_t(1.f))
        && (coords[2] > scalar_t(0.f)) && (coords[2] < scalar_t(1.f))
    };
    return isInside;
}


std::unique_ptr<Vector2>
algo::
isOverlapping(Triangle2 const & tri_1, Triangle2 const & tri_2)
{
    for (int v_id{0}; v_id < 3; v_id++)
    {
        if (isPointInsideTriangle(tri_1, tri_2[v_id]))
        {
            return std::make_unique<Vector2>(tri_2[v_id]);
        }
    }
    return nullptr;
}


std::unique_ptr<Vector2>
algo::
areOverlapping(TriangleGroup const & group1, TriangleGroup const & group2)
{
    if (!algo::getBoundingBox(group1).overlaps(algo::getBoundingBox(group2)))
    {
        return nullptr;
    }

    //- Brute force
    auto const segments1 = group1.getAbsBoundary();
    auto const segments2 = group2.getAbsBoundary();

    for (Segment2 const & s1 : segments1)
    {
        Vector2 const dir{s1[1] - s1[0]};
        Vector2 const outward_normal{dir[1], -dir[0]};
        for(Segment2 const & s2 : segments2)
        {
            if (dot(s2[0] - s1[0], outward_normal) < 0)
            {
                return std::make_unique<Vector2>(s2[0]);
            }
            else if (dot(s2[1] - s1[0], outward_normal) < 0)
            {
                return std::make_unique<Vector2>(s2[1]);
            }
        }
    }
    return nullptr;
}


scalar_t
algo::
pointToSegmentDistance(Segment2 const & s, Vector2 const & pt)
{
    Vector2 const segment_direction{s[1] - s[0]};
    Vector2 const outward_normal{segment_direction[1], -segment_direction[0]};
    return dot(pt - s[0], outward_normal);
}


bool
algo::
isPointInsidePolygon(TriangleGroup const & g, Vector2 const & pt)
{
    for (Segment2 const & s : g.getAbsBoundary())
    {
        if (pointToSegmentDistance(s, pt) < 0)
        {
            return true;
        }
    }
    return false;
}
