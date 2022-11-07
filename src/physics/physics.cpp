#include "physics.h"

#include "collision.h"
#include "geometry/triangle_algo.h"

#include <numeric>
#include <set>


namespace
{
std::chrono::high_resolution_clock::time_point timeNow()
{
    return std::chrono::high_resolution_clock::now();
}


scalar_t timeSince(std::chrono::high_resolution_clock::time_point const & ref_time)
{
    using duration = std::chrono::duration<scalar_t>;
    duration time_counts = timeNow() - ref_time;
    return time_counts.count();
}
}


Physics &
Physics::
getSingleton()
{
    static Physics instance{std::chrono::high_resolution_clock::now()};
    return instance;
}


void
Physics::
advanceGroup(TriangleGroup & tri_group, scalar_t const dt) const
{
    tri_group.position += tri_group.velocity * dt;
    // TODO: Handle external forces
    // tri_group.velocity += Physics::externalForces(tri_group) * dt;
    // TODO: Handle orientation
}


void
Physics::
advance()
{
    scalar_t const dt = timeSince(last_time);
    last_time = timeNow();

    std::vector<TriangleGroup> & tri_groups{tri_manager.getTriangleGroups()};

    for (auto & tri_group : tri_groups)
    {
        advanceGroup(tri_group, dt);
    }

    for (auto & tri_group : tri_groups)
    {
        if (tri_group.position[0] > 1)
        {
            algo::rigidWallCollision(Vector2{-1, 0}, tri_group.velocity);
        }
        else if (tri_group.position[1] > 1)
        {
            algo::rigidWallCollision(Vector2{0, -1}, tri_group.velocity);
        }
        else if (tri_group.position[0] < -1)
        {
            algo::rigidWallCollision(Vector2{1, 0}, tri_group.velocity);
        }
        else if (tri_group.position[1] < -1)
        {
            algo::rigidWallCollision(Vector2{0, 1}, tri_group.velocity);
        }
    }

    if (tri_groups.size() <= 1) return;

    // TODO: Use nearest-neighbours to optimize this code
    for(label_t gid{0}; gid != tri_groups.size() - 1; ++gid)
    {
        for(label_t ngid{gid + 1}; ngid != tri_groups.size(); ++ngid)
        {
            if (algo::areOverlapping(tri_groups[gid], tri_groups[ngid]))
            {
                algo::pointMassRigidCollision
                (
                    tri_groups[gid].area, tri_groups[gid].position,  tri_groups[gid].velocity,
                    tri_groups[ngid].area,tri_groups[ngid].position, tri_groups[ngid].velocity
                );
                break;
            }
        }
    }
}


Physics::
Physics(std::chrono::high_resolution_clock::time_point const & now)
    : start_time{now}
    , last_time{now}
    , tri_manager(TrianglesManager::getSingleton())
{};
