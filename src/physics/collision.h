#ifndef COLLISION_H_
#define COLLISION_H_


#include "math/vector_math.h"


namespace algo
{

void pointMassRigidCollision
(
    scalar_t const mass1, Vector2 const & position1, Vector2 & velocity1,
    scalar_t const mass2, Vector2 const & position2, Vector2 & velocity2
);


void rigidWallCollision
(
    Vector2 const & wall_normal, Vector2 & velocity
);

}

#endif