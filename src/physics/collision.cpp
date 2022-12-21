#include "collision.h"

#include "geometry/boundary_manager.h"
#include "geometry/coordinate_transformation.h"
#include "geometry/triangle_algo.h"
#include "geometry/triangle_group.h"
#include "geometry/triangles_manager.h"
#include "geometry/quad_tree.h"

#include <math.h>
#include <set>
#include <iostream>


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

    static constexpr scalar_t elasticity = 1.0;

}


Vector2 getNormalAtPt(TriangleGroup & g1, TriangleGroup & g2, Vector2 & contact_pt)
{
    scalar_t dt = 1E-2;
    std::unique_ptr<Vector2> pt;

    Vector2 const p1{g1.position};
    Vector2 const p2{g2.position};

    scalar_t const a1{g1.orientation};
    scalar_t const a2{g2.orientation};

    bool early{true}, late{true};
    while (dt > 1E-4 || early || late)
    {
        if (auto pt = algo::areOverlapping(g1, g2))
        {
            late = false;
            contact_pt = *pt;
            dt *= 1.5;
            g1.position -= g1.velocity*dt;
            g1.orientation -= g1.angular_velocity*dt;

            g2.position -= g2.velocity*dt;
            g2.orientation -= g2.angular_velocity*dt;
        }
        else
        {
            early = false;
            dt /= 3;
            g1.position += g1.velocity*dt;
            g1.orientation += g1.angular_velocity*dt;

            g2.position += g2.velocity*dt;
            g2.orientation += g2.angular_velocity*dt;
        }
    }

    g1.position = p1;
    g2.position = p2;

    g1.orientation = a1;
    g2.orientation = a2;

    scalar_t min_dst{SCALAR_MAX};
    Vector2 segment_direction;
    for (auto const & s : g1.getAbsBoundary())
    {
        scalar_t dst {abs(algo::pointToSegmentDistance(s, contact_pt))};
        if (dst < min_dst)
        {
            // orientation assumed counter clockwise
            segment_direction = s[1] - s[0];
            std::swap(min_dst, dst);
        }
    }
    Vector2 const outward_normal{segment_direction[1], -segment_direction[0]};
    return unitVector(outward_normal);
}


void
algo::
rigidBodyCollision
(
    TriangleGroup & tri_1,
    TriangleGroup & tri_2
)
{
    Vector2 const system_momentum
    {
          tri_1.velocity*tri_1.area
        + tri_2.velocity*tri_2.area
    };
    scalar_t const system_angular_momentum
    {
          tri_1.angular_velocity*tri_1.moment_of_inertia
        + tri_2.angular_velocity*tri_2.moment_of_inertia
    };
    /*
    Impulse-based reaction model
    https://en.wikipedia.org/wiki/Collision_response#Impulse-based_contact_model
    */

    Vector2 contact_pt;
    Vector2 const normal{getNormalAtPt(tri_1, tri_2, contact_pt)};  // outward normal of tri_1

    // Relative velocity at contact pt
    Vector2 rel_pos_1 {algo::toRelative(tri_1, contact_pt)};
    Vector2 rel_pos_2 {algo::toRelative(tri_2, contact_pt)};

    Vector2 const v_contact_1{velocityOfPointOnRigidBody(tri_1, rel_pos_1)};
    Vector2 const v_contact_2{velocityOfPointOnRigidBody(tri_2, rel_pos_2)};

    auto helper = [&normal](Vector2 const & r) -> scalar_t
    {
        // Evaluating a convoluted cross product: ((r x n) x r).dot(n)
        return r[0]*r[0]*normal[1]*normal[1]
             + r[1]*r[1]*normal[0]*normal[0]
             - 2.0f*r[0]*r[1]*normal[0]*normal[1];
    };

    auto const vrel{dot(normal, v_contact_2 - v_contact_1)};

    if (vrel > 0) return;

    scalar_t const numerator = -(1+elasticity)*vrel;

    scalar_t const denominator =
          1.0f/tri_1.area
        + 1.0f/tri_2.area
        + helper(rel_pos_1) / tri_1.moment_of_inertia
        + helper(rel_pos_2) / tri_2.moment_of_inertia;

    static constexpr scalar_t delta[2] =
    {
        -std::numeric_limits<scalar_t>::epsilon(),
         std::numeric_limits<scalar_t>::epsilon()
    };

    scalar_t const impulse_magnitude
    {
        numerator / (denominator + delta[denominator > 0])
    };

    Vector2 const impulse{impulse_magnitude * normal};
    tri_1.velocity -= impulse / tri_1.area;
    tri_2.velocity = (system_momentum - tri_1.velocity*tri_1.area)/tri_2.area;

    tri_1.angular_velocity -= impulse_magnitude/tri_1.moment_of_inertia*(rel_pos_1[0]*normal[1] - normal[0]*rel_pos_1[1]);
    tri_2.angular_velocity = (system_angular_momentum - tri_1.angular_velocity*tri_1.moment_of_inertia) / tri_2.moment_of_inertia;
}


bool
algo::
rigidWallCollision
(
    Vector2 const & inward_wall_normal, Vector2 & velocity
)
{
    scalar_t const v1_n {dot(velocity, inward_wall_normal)};
    if (v1_n < 0)
    {
        velocity -= elasticity*inward_wall_normal*(v1_n*2);
        return true;
    }
    return false;
}


bool
algo::
boundaryCollision(TriangleGroup & group)
{
    static BoundariesManager & bdry_manager{BoundariesManager::getSingleton()};
    for (Segment2 const & boundary : bdry_manager.getBoundary())
    {
        Vector2 const segment_direction{boundary[1] - boundary[0]};
        Vector2 const inward_normal{-segment_direction[1], segment_direction[0]};

        if ( dot(group.position - boundary[0], inward_normal) < 0 )  // This test is only valid if boundary_segments form a convex polygon!!!
        {
            //- This is the segment to collide with
            if (rigidWallCollision(unitVector(inward_normal), group.velocity))
            {
                return true;
            }
        }
    }
    return false;
}


void
algo::
handleCollisions()
{
    static BoundariesManager & bdry_manager {BoundariesManager::getSingleton()};
    static TrianglesManager & tri_manager{TrianglesManager::getSingleton()};
    std::vector<TriangleGroup> & tri_groups{tri_manager.getTriangleGroups()};

    for(auto & group : tri_groups)
    {
        boundaryCollision(group);
    }

    QuadTree qt{bdry_manager.getBoundingBox()};
    for (auto & group : tri_groups)
    {
        qt.insert(QuadTreeNode::make(group.position, group.ptr.get()));
    }

    // for (label_t i=0; i < tri_groups.size() - 1; ++i)
    // {
    //     for (label_t j=i+1; j < tri_groups.size(); ++j)
    //     {
    //         if (auto pt = algo::areOverlapping(tri_groups[i], tri_groups[j]))
    //         {
    //             rigidBodyCollision(tri_groups[i], tri_groups[j]);
    //             break;
    //         }
    //     }
    // }

    std::set<TriangleGroup const *> collided;
    for (auto & group : tri_groups)
    {
        if (collided.find(&group) != collided.end()) continue;

        auto found_groups {qt.query(algo::getBoundingBox(group, group.influence_radius))};
        for (auto & neighbour_node : found_groups)
        {
            TriangleGroup ** neighbour {static_cast<TriangleGroup**>(neighbour_node.data)};
            if (&group == *neighbour) [[unlikely]]
            {
                continue;
            }
            else if(collided.find(*neighbour) != collided.end())
            {
                continue;
            }
            else if (auto pt = algo::areOverlapping(group, **neighbour))
            {
                rigidBodyCollision(group, **neighbour);
                collided.insert(&group);
                collided.insert(*neighbour);
                break;
            }
        }
    }
}