#include "physics.h"

#include "collision.h"
#include "geometry/quad_tree.h"
#include "geometry/triangle_algo.h"
#include "geometry/triangle_group.h"

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

    scalar_t energy = 0;
    scalar_t linMom = 0;

    auto computeEnergy = [](TriangleGroup const & g) -> scalar_t
    {
        return 0.5*(g.area*magSqr(g.velocity)) + 0.5*g.moment_of_inertia*g.angular_velocity*g.angular_velocity;
    };

    auto linMomentum = [](TriangleGroup const & g) -> scalar_t
    {
        return g.area*mag(g.velocity);
    };

    for (auto & tri_group : tri_groups)
    {
        energy += computeEnergy(tri_group);
        linMom += linMomentum(tri_group);
        advanceGroup(tri_group, dt);
    }

    std::cout << "Time elapsed: " << timeSince(start_time)
              << " FPS=" << 1.f/dt << std::endl
              << "energy: " << energy
              << " momentum: " << linMom
              << std::endl;

    for(auto & group : tri_groups)
    {
        algo::boundaryCollision(bdry_manager.getBoundary(), group);
    }

    QuadTree qt{bdry_manager.getBoundingBox()};
    for (auto & group : tri_groups)
    {
        qt.insert(QuadTreeNode::make(group.position, group.ptr.get()));
    }

    for (auto & group : tri_groups)
    {
        auto found_groups {qt.query(algo::getBoundingBox(group, 2*group.influence_radius))};
        for (auto & neighbour_node : found_groups)
        {
            TriangleGroup ** neighbour
            {
                static_cast<TriangleGroup**>(neighbour_node.data)
            };
            if (algo::areOverlapping(group, **neighbour))
            {
                algo::pointMassRigidCollision(group, **neighbour);
                break;
            }
        }
    }
}


Physics::
Physics(std::chrono::high_resolution_clock::time_point const & now)
    : start_time{now}
    , last_time{now}
    , tri_manager{TrianglesManager::getSingleton()}
    , bdry_manager{BoundariesManager::getSingleton()}
{};
