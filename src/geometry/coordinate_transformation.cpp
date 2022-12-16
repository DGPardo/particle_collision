#include "coordinate_transformation.h"
#include <memory>

Vector2
algo::
rotatePoint(scalar_t angle_rads, Vector2 const & pt)
{
    // Rotates counter clockwise if angle_rads > 0
    return Vector2
    {
        pt[0]*cos(angle_rads) - pt[1]*sin(angle_rads),
        pt[0]*sin(angle_rads) + pt[1]*cos(angle_rads)
    };
}


Vector2
algo::
toRelative(TriangleGroup const & group, Vector2 const & pt)
{
    return rotatePoint(-group.orientation, pt - group.position);
}


Vector2
algo::
toAbsolute(TriangleGroup const & group, Vector2 const & pt)
{
    Vector2 rpt = rotatePoint(group.orientation, pt);
    return rpt + group.position;
}
