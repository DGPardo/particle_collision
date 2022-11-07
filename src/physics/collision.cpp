#include "collision.h"

#include <math.h>


void
algo::
pointMassRigidCollision
(
    scalar_t const mass1, Vector2 const & position1, Vector2 & velocity1,
    scalar_t const mass2, Vector2 const & position2, Vector2 & velocity2
)
{
    Vector2 const normal{unitVector(position2 - position1)};
    Vector2 const tangent{-normal[1], normal[0]};

    scalar_t const twoK {mass1*magSqr(velocity1) + mass2*magSqr(velocity2)};  // 2 * kinetic energy

    // Initial velocities prior to collide
    scalar_t const v1_n {dot(velocity1, normal)};
    scalar_t const v2_n {dot(velocity2, normal)};
    scalar_t const m_n {v1_n*mass1 + v2_n*mass2};  // linear momentum along normal direction

    scalar_t const v1_t {dot(velocity1, tangent)};
    scalar_t const v2_t {dot(velocity2, tangent)};

    /*
    Using momentum conservation and energy conservation
    it is possible to find a solution to this problem.
    We assume no friction and therefore the tangential component
    of linear momentum is constant. Only a balance in the normal
    component needs to be solved.
    This leads to a quadratic equation. TODO: Investigate two solutions
    */

    // Quadratic Equation [a, b, c] * [v2_n^2, v2_n, 1]
    scalar_t const a
    {
        mass2*mass2/mass1 + mass2
    };
    scalar_t const b
    {
        scalar_t(-2.0)*m_n*mass2/mass1
    };
    scalar_t const c
    {
        -twoK + mass1*v1_t*v1_t + mass2*v2_t*v2_t + m_n*m_n/mass1
    };

    //- Velocities after collision
    scalar_t const discriminant{b*b - 4*a*c};
    scalar_t const v2_nf = (-b + sqrt(discriminant)) / (2*a);
    scalar_t const v1_nf = (m_n - mass2*v2_nf) / mass1;

    velocity1 = (v1_t*tangent) + (v1_nf*normal);
    velocity2 = v2_t*tangent + v2_nf*normal;
}


void
algo::rigidWallCollision
(
    Vector2 const & wall_normal, Vector2 & velocity
)
{
    scalar_t const v1_n {dot(velocity, wall_normal)};
    velocity -= wall_normal*(v1_n*2);
}

