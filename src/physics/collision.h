#ifndef COLLISION_H_
#define COLLISION_H_


#include "math/vector_math.h"
#include "geometry/segment.h"
#include <vector>


class TriangleGroup;


namespace algo
{

void pointMassRigidCollision
(
    TriangleGroup & tri_1,
    TriangleGroup & tri_2
);


void rigidWallCollision
(
    Vector2 const & inward_wall_normal, Vector2 & velocity
);


void boundaryCollision
(
    // - Boundary must be a convex polygon
    std::vector<Segment2> const & boundary_segments, TriangleGroup & g
);


}

#endif