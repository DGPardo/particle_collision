#include "physics.h"

#include "collision.h"
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
    tri_group.orientation += tri_group.angular_velocity*dt;
    
    tri_group.velocity[1] -= 9.81*dt;  // Gravity

    //- Modelling Air Friction
    // tri_group.velocity -= unitVector(tri_group.velocity) *0.5*1.225*magSqr(tri_group.velocity)*0.001;
    // tri_group.angular_velocity -= tri_group.angular_velocity/(abs(tri_group.angular_velocity) + TOL) *0.5*1.225*tri_group.angular_velocity*tri_group.angular_velocity*0.01;
}


void
Physics::
advance()
{
    scalar_t const dt = timeSince(last_time);
    last_time = timeNow();
    std::cout << "Time elapsed: " << timeSince(start_time) << "  FPS=" << 1.f/dt << std::endl;
    
    std::vector<TriangleGroup> & tri_groups{tri_manager.getTriangleGroups()};

    scalar_t energy = 0;
    scalar_t linMom = 0;
    scalar_t angMom = 0;
    
    auto computeEnergy = [](TriangleGroup const & g) -> scalar_t
    {
        return 0.5*(g.area*magSqr(g.velocity)) + 0.5*g.moment_of_inertia*g.angular_velocity*g.angular_velocity;
    };

    auto linMomentum = [](TriangleGroup const & g) -> scalar_t
    {
        return g.area*mag(g.velocity);
    };

    auto angMomentum = [](TriangleGroup const & g) -> scalar_t
    {
        return g.moment_of_inertia*g.angular_velocity;
    };

    for (auto & tri_group : tri_groups)
    {
        energy += computeEnergy(tri_group);
        linMom += linMomentum(tri_group);
        angMom += angMomentum(tri_group);
        advanceGroup(tri_group, dt);
    }
    std::cout << "energy: " << energy << "  lin: " << linMom << "  ang: " << angMom << std::endl;

    algo::handleCollisions();
}


Physics::
Physics(std::chrono::high_resolution_clock::time_point const & now)
    : start_time{now}
    , last_time{now}
    , tri_manager{TrianglesManager::getSingleton()}
    , bdry_manager{BoundariesManager::getSingleton()}
{};



