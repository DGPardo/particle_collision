#ifndef COLLISION_H_
#define COLLISION_H_


#include "math/vector_math.h"
#include "geometry/segment.h"
#include <vector>


class TriangleGroup;


namespace algo
{

void rigidBodyCollision
(
    TriangleGroup & tri_1,
    TriangleGroup & tri_2
);


bool rigidWallCollision
(
    Vector2 const & inward_wall_normal, Vector2 & velocity
);


bool boundaryCollision
(
    // - Boundary must be a convex polygon
    TriangleGroup & group
);


void handleCollisions();

}

#endif