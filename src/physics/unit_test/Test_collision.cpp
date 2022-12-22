#include "gtest/gtest.h"

#include "geometry/boundary_manager.h"
#include "geometry/polygon.h"
#include "geometry/triangle_group.h"
#include "geometry/triangles_manager.h"

#include "physics/collision.h"


#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*100)


TEST(Collision, pointMassRigidCollision_LeftToRight_OneDimensional)
{
    scalar_t const mass1{1};
    scalar_t const mass2{1};

    Vector2 position1{0, 0};
    Vector2 position2{1, 0};

    Vector2 velocity1{1, 0};
    Vector2 velocity2{0, 0};

    TriangleGroup tri_1{position1, velocity1};
    TriangleGroup tri_2{position2, velocity2};

    tri_1.area = mass1;
    tri_2.area = mass2;

    algo::pointMassRigidCollision(tri_1, tri_2);

    Vector2 const exp_tri1_velocity{0, 0};
    Vector2 const exp_tri2_velocity{1, 0};

    ASSERT_NEAR(tri_1.velocity[0], exp_tri1_velocity[0], TOL);
    ASSERT_NEAR(tri_1.velocity[1], exp_tri1_velocity[1], TOL);

    ASSERT_NEAR(tri_2.velocity[0], exp_tri2_velocity[0], TOL);
    ASSERT_NEAR(tri_2.velocity[1], exp_tri2_velocity[1], TOL);
}


TEST(Collision, pointMassRigidCollision_LeftToRight_OneDimensional_Inverted)
{
    scalar_t const mass1{1};
    scalar_t const mass2{1};

    Vector2 position1{1, 0};
    Vector2 position2{0, 0};

    Vector2 velocity1{-1, 0};
    Vector2 velocity2{0, 0};

    TriangleGroup tri_1{position1, velocity1};
    TriangleGroup tri_2{position2, velocity2};

    tri_1.area = mass1;
    tri_2.area = mass2;

    algo::pointMassRigidCollision(tri_1, tri_2);

    Vector2 const exp_tri1_velocity{0, 0};
    Vector2 const exp_tri2_velocity{-1, 0};

    ASSERT_NEAR(tri_1.velocity[0], exp_tri1_velocity[0], TOL);
    ASSERT_NEAR(tri_1.velocity[1], exp_tri1_velocity[1], TOL);

    ASSERT_NEAR(tri_2.velocity[0], exp_tri2_velocity[0], TOL);
    ASSERT_NEAR(tri_2.velocity[1], exp_tri2_velocity[1], TOL);
}


TEST(Collision, pointMassRigidCollision_RightToLeft_OneDimensional_InvertedOrder)
{
    scalar_t const mass1{1};
    scalar_t const mass2{1};

    Vector2 position1{1, 0};
    Vector2 position2{0, 0};

    Vector2 velocity1{-1, 0};
    Vector2 velocity2{0, 0};

    TriangleGroup tri_1{position1, velocity1};
    TriangleGroup tri_2{position2, velocity2};

    tri_1.area = mass1;
    tri_2.area = mass2;

    algo::pointMassRigidCollision(tri_1, tri_2);

    Vector2 const exp_tri1_velocity{0, 0};
    Vector2 const exp_tri2_velocity{-1, 0};

    ASSERT_NEAR(tri_1.velocity[0], exp_tri1_velocity[0], TOL);
    ASSERT_NEAR(tri_1.velocity[1], exp_tri1_velocity[1], TOL);

    ASSERT_NEAR(tri_2.velocity[0], exp_tri2_velocity[0], TOL);
    ASSERT_NEAR(tri_2.velocity[1], exp_tri2_velocity[1], TOL);
}


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


