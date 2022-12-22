#include "gtest/gtest.h"

#include "geometry/coordinate_transformation.h"


#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*100)


TEST(CoordinateTransformations, transformCoordinate)
{
    //- Test input scenario
    constexpr Vector2 pos{1, 0};

    //- Test expected results
    constexpr Vector2 expected{-1, 0};

    //- Evaluate test function
    Vector2 const result {algo::rotatePoint(M_PI, pos)};

    //- Assertions
    ASSERT_NEAR(result[0], expected[0], TOL);
    ASSERT_NEAR(result[1], expected[1], TOL);
}