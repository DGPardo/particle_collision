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
    algo::boundaryCollision(g);

    //- Assertions
    ASSERT_NEAR(expected_velocity[0], g.velocity[0], 2*TOL);
    ASSERT_NEAR(expected_velocity[1], g.velocity[1], 2*TOL);
}


TEST(Collision, boundaryWallCollision2)
{
    //- Test input scenario
    auto & bdry_manager {BoundariesManager::getSingleton()};
    constexpr ConvexPolygon<4> domain{makeCircle<4>(scalar_t(1))}; // with so few vertices, this is a square rotated 45 degs
    bdry_manager.setBoundary(domain);

    auto & tri_manager {TrianglesManager::getSingleton()};
    constexpr auto circle {makeCircle<36>(1)};
    tri_manager.addGroup
    (
        {{0.001, 1.001}},  // Just outside
        {{0, 0.5}},
        circle
    );
    TriangleGroup & g {tri_manager.getTriangleGroups().back()};

    //- Test expected results
    Vector2 const expected_velocity{-0.5, 0};

    //- Evaluate test function
    algo::boundaryCollision(g);

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
    algo::boundaryCollision(g);

    //- Assertions
    ASSERT_NEAR(expected_velocity[0], g.velocity[0], 1E-4);
    ASSERT_NEAR(expected_velocity[1], g.velocity[1], 1E-4);
}


TEST(Collision, bodyToBodyCollision)
{
    auto & tri_manager {TrianglesManager::getSingleton()};

    constexpr Vector2 c1{-0.1, -0.02};
    constexpr Vector2 c2{0.1, 0.02};
    constexpr auto bar{Rectangle(c1, c2)};

    tri_manager.addGroup
    (
        {{0, 0}},
        {{0, -0.1}},
        bar
    );

    TriangleGroup & g1 {tri_manager.getTriangleGroups().back()};

    tri_manager.addGroup
    (
        {{0.1, -0.039}},
        {{0, 0}},
        bar
    );

    TriangleGroup & g2 {tri_manager.getTriangleGroups().back()};

    auto pt {algo::areOverlapping(g1, g2)};
    ASSERT_TRUE(pt.get() != nullptr);

    auto getAngularMomentum = [&g1, &g2]() -> scalar_t
    {
        return g1.angular_velocity*g1.moment_of_inertia
             + g2.angular_velocity*g2.moment_of_inertia;
    };

    scalar_t before_collision{getAngularMomentum()};
    algo::rigidBodyCollision(g1, g2);
    scalar_t after_collision{getAngularMomentum()};

    ASSERT_NEAR(before_collision, after_collision, TOL);
}