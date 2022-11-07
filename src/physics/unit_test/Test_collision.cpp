#include "gtest/gtest.h"

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

    algo::pointMassRigidCollision(mass1, position1, velocity1, mass2, position2, velocity2);

    Vector2 const exp_tri1_velocity{0, 0};
    Vector2 const exp_tri2_velocity{1, 0};

    ASSERT_NEAR(velocity1[0], exp_tri1_velocity[0], TOL);
    ASSERT_NEAR(velocity1[1], exp_tri1_velocity[1], TOL);

    ASSERT_NEAR(velocity2[0], exp_tri2_velocity[0], TOL);
    ASSERT_NEAR(velocity2[1], exp_tri2_velocity[1], TOL);
}


TEST(Collision, pointMassRigidCollision_LeftToRight_OneDimensional_Inverted)
{
    scalar_t const mass1{1};
    scalar_t const mass2{1};

    Vector2 position1{1, 0};
    Vector2 position2{0, 0};

    Vector2 velocity1{-1, 0};
    Vector2 velocity2{0, 0};

    algo::pointMassRigidCollision(mass1, position1, velocity1, mass2, position2, velocity2);

    Vector2 const exp_tri1_velocity{0, 0};
    Vector2 const exp_tri2_velocity{-1, 0};

    ASSERT_NEAR(velocity1[0], exp_tri1_velocity[0], TOL);
    ASSERT_NEAR(velocity1[1], exp_tri1_velocity[1], TOL);

    ASSERT_NEAR(velocity2[0], exp_tri2_velocity[0], TOL);
    ASSERT_NEAR(velocity2[1], exp_tri2_velocity[1], TOL);
}


TEST(Collision, pointMassRigidCollision_RightToLeft_OneDimensional_InvertedOrder)
{
    scalar_t const mass1{1};
    scalar_t const mass2{1};

    Vector2 position1{1, 0};
    Vector2 position2{0, 0};

    Vector2 velocity1{-1, 0};
    Vector2 velocity2{0, 0};

    algo::pointMassRigidCollision(mass1, position1, velocity1, mass2, position2, velocity2);

    Vector2 const exp_tri1_velocity{0, 0};
    Vector2 const exp_tri2_velocity{-1, 0};

    ASSERT_NEAR(velocity1[0], exp_tri1_velocity[0], TOL);
    ASSERT_NEAR(velocity1[1], exp_tri1_velocity[1], TOL);

    ASSERT_NEAR(velocity2[0], exp_tri2_velocity[0], TOL);
    ASSERT_NEAR(velocity2[1], exp_tri2_velocity[1], TOL);
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



