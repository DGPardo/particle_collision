#include "gtest/gtest.h"

#include "geometry/boundary_manager.h"
#include "geometry/polygon.h"
#include "geometry/triangle_group.h"
#include "geometry/triangles_manager.h"

#include "physics/collision.h"


#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*100)


TEST(Collision, rigidWallCollision)
{
    //- Test input scenario
    Vector2 velocity{-1, 1};
    Vector2 const wall_normal{1, 0};

    //- Test expected results
    Vector2 const expected_velocity{1, 1};

    //- Evaluate test function
    algo::rigidWallCollision(wall_normal, velocity);

    //- Assertions
    ASSERT_NEAR(expected_velocity[0], velocity[0], TOL);
    ASSERT_NEAR(expected_velocity[1], velocity[1], TOL);
}


TEST(Collision, boundaryWallCollision)
{
    //- Test input scenario
    auto & bdry_manager {BoundariesManager::getSingleton()};
    constexpr ConvexPolygon<4> domain{makeCircle<4>(scalar_t(1))}; // with so few vertices, this is a square rotated 45 degs
    bdry_manager.setBoundary(domain);

    auto & tri_manager {TrianglesManager::getSingleton()};
    constexpr auto circle {makeCircle<36>(1)};
    tri_manager.addGroup
    (
        {{1/sqrt(2) + TOL, 1/sqrt(2) + TOL}},  // Just outside
        {{0.5, 0.5}},
        circle
    );
    TriangleGroup & g {tri_manager.getTriangleGroups()[0]};

    //- Test expected results
    Vector2 const expected_velocity{-0.5, -0.5};

    //- Evaluate test function
    algo::boundaryCollision(bdry_manager.getBoundary(), g);

    //- Assertions
    ASSERT_NEAR(expected_velocity[0], g.velocity[0], TOL);
    ASSERT_NEAR(expected_velocity[1], g.velocity[1], TOL);
}


TEST(Collision, boundaryWallCollisionRectangle)
{
    //- Test input scenario
    auto & bdry_manager {BoundariesManager::getSingleton()};
    
    constexpr Vector2 ll{-1, -1};
    constexpr Vector2 ur{1, 1};
    constexpr auto domain{Rectangle(ll, ur)};

    bdry_manager.setBoundary(domain);

    auto & tri_manager {TrianglesManager::getSingleton()};
    constexpr auto circle {makeCircle<36>(1)};

    tri_manager.addGroup
    (
        {{0, 1.1f}},  // Just outside
        {{0, 0.5}},
        circle
    );
    TriangleGroup & g {tri_manager.getTriangleGroups().back()};

    //- Test expected results
    Vector2 const expected_velocity{0, -0.5};

    //- Evaluate test function
    algo::boundaryCollision(bdry_manager.getBoundary(), g);

    //- Assertions
    ASSERT_NEAR(expected_velocity[0], g.velocity[0], TOL);
    ASSERT_NEAR(expected_velocity[1], g.velocity[1], TOL);
}
