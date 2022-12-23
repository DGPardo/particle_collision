#include "collision.h"

#include <math.h>
#include "geometry/triangle_algo.h"
#include "geometry/triangle_group.h"


void
algo::
pointMassRigidCollision
(
    TriangleGroup & tri_1,
    TriangleGroup & tri_2
)
{
    Vector2 const normal{unitVector(tri_2.position - tri_1.position)};
    Vector2 const tangent{-normal[1], normal[0]};

    scalar_t const twoK {tri_1.area*magSqr(tri_1.velocity) + tri_2.area*magSqr(tri_2.velocity)};  // 2 * kinetic energy

    // Initial velocities prior to collide
    scalar_t const v1_n {dot(tri_1.velocity, normal)};
    scalar_t const v2_n {dot(tri_2.velocity, normal)};
    scalar_t const m_n {v1_n*tri_1.area + v2_n*tri_2.area};  // linear momentum along normal direction

    scalar_t const v1_t {dot(tri_1.velocity, tangent)};
    scalar_t const v2_t {dot(tri_2.velocity, tangent)};

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
        tri_2.area*tri_2.area/tri_1.area + tri_2.area
    };
    scalar_t const b
    {
        scalar_t(-2.0)*m_n*tri_2.area/tri_1.area
    };
    scalar_t const c
    {
        -twoK + tri_1.area*v1_t*v1_t + tri_2.area*v2_t*v2_t + m_n*m_n/tri_1.area
    };

    //- Velocities after collision
    scalar_t const discriminant{b*b - 4*a*c};
    scalar_t const v2_nf = (-b + sqrt(discriminant)) / (2*a);
    scalar_t const v1_nf = (m_n - tri_2.area*v2_nf) / tri_1.area;

    tri_1.velocity = (v1_t*tangent) + (v1_nf*normal);
    tri_2.velocity = v2_t*tangent + v2_nf*normal;
}


void
algo::
rigidWallCollision
(
    Vector2 const & inward_wall_normal, Vector2 & velocity
)
{
    scalar_t const v1_n {dot(velocity, inward_wall_normal)};
    if (v1_n < 0)
    {
        velocity -= inward_wall_normal*(v1_n*2);
    }
}


void
algo::
boundaryCollision(std::vector<Segment2> const & boundary_segments, TriangleGroup & g)
{
    for (Segment2 const & boundary : boundary_segments)
    {
        Vector2 const segment_direction{boundary[1] - boundary[0]};
        Vector2 const inward_normal{-segment_direction[1], segment_direction[0]};
        Vector2 const ref_pt{boundary[0] + 0.5*inward_normal*g.influence_radius};  // accounting for ball radius
        if ( dot(g.position - ref_pt, inward_normal) < 0 )  // This test is only valid if boundary_segments form a convex polygon!!!
        {
            //- This is the segment to collide with
            rigidWallCollision(unitVector(inward_normal), g.velocity);
            return;
        }
    }
}
