#include "collision.h"

#include <math.h>
#include "geometry/triangle_algo.h"
#include "geometry/triangle_group.h"
#include "geometry/coordinate_transformation.h"

namespace
{
    Vector2 velocityOfPointOnRigidBody(TriangleGroup const & rigid_body, Vector2 const & relative_position)
    {
        // relative_position must be the position w.r.t to the rigid_body centroid/"position" (rotation center)
        return Vector2
        {
            -relative_position[1]*rigid_body.angular_velocity + rigid_body.velocity[0],
             relative_position[0]*rigid_body.angular_velocity + rigid_body.velocity[1]
        };
    }
}


Vector2 getNormalAtPt(TriangleGroup const & rigid_body, Vector2 const & rel_pt)
{
    scalar_t min_dst{1};
    Vector2 edge_direction;
    for (Segment2 const & s : rigid_body.getBoundary())
    {
        scalar_t dst = algo::isPointOnASegment(s, rel_pt);
        if (dst < min_dst)
        {
            edge_direction = s[1] - s[0];
            min_dst = dst;
        }
    }
    return unitVector(Vector2{-edge_direction[1], edge_direction[0]});
}


void
algo::
rigidBodyCollision
(
    TriangleGroup & tri_1,
    TriangleGroup & tri_2,
    Vector2 const & contact_pt
)
{
    /*
    Impulse-based reaction model
    https://en.wikipedia.org/wiki/Collision_response#Impulse-based_contact_model
    */

    // Relative velocity at contact pt
    Vector2 rel_pos_1 {algo::toRelative(tri_1, contact_pt)};
    Vector2 rel_pos_2 {algo::toRelative(tri_2, contact_pt)};

    Vector2 const v_contact_1{velocityOfPointOnRigidBody(tri_1, rel_pos_1)};
    Vector2 const v_contact_2{velocityOfPointOnRigidBody(tri_2, rel_pos_2)};
    Vector2 const normal{getNormalAtPt(tri_1, rel_pos_1)};

    auto helper = [&normal](Vector2 const & r) -> scalar_t
    {
        // Evaluating a convoluted cross product: ((r x n) x r).dot(n)
        return r[0]*r[0]*normal[1]*normal[1]
             + r[1]*r[1]*normal[0]*normal[0]
             - 2.0f*r[0]*r[1]*normal[0]*normal[1];
    };

    constexpr scalar_t e = 1; // elasticity of the impact
    scalar_t const numerator = -(1+e)*dot(normal, v_contact_2 - v_contact_1);

    scalar_t const denominator =
          1.0f/tri_1.area
        + 1.0f/tri_2.area
        + helper(rel_pos_1) / tri_1.moment_of_inertia
        + helper(rel_pos_2) / tri_2.moment_of_inertia;

    scalar_t const impulse_magnitude
    {
        numerator /
        (
            denominator > 0  // handling possible division by zero
            ? denominator + std::numeric_limits<scalar_t>::epsilon()
            : denominator - std::numeric_limits<scalar_t>::epsilon()
        )
    };

    Vector2 const impulse{impulse_magnitude * normal};
    tri_1.velocity -= impulse / tri_1.area;
    tri_2.velocity += impulse / tri_2.area;

    tri_1.angular_velocity -= impulse_magnitude/tri_1.moment_of_inertia*(rel_pos_1[0]*normal[1] - normal[0]*rel_pos_1[1]);
    tri_2.angular_velocity += impulse_magnitude/tri_2.moment_of_inertia*(rel_pos_2[0]*normal[1] - normal[0]*rel_pos_2[1]);
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

        if ( dot(g.position - boundary[0], inward_normal) < 0 )  // This test is only valid if boundary_segments form a convex polygon!!!
        {
            //- This is the segment to collide with
            rigidWallCollision(unitVector(inward_normal), g.velocity);
            break;
        }
    }
}
