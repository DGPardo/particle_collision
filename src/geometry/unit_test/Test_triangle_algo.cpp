#include "gtest/gtest.h"

#include "geometry/triangle.h"
#include "geometry/triangle_algo.h"
#include "geometry/polygon.h"

#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*100)


TEST(TriangleAlgo, baryCentricCoordinates)
{
    //- Test input scenario
    Triangle2 triangle{{{-0.5,0}, {0.5,0}, {0,1}}};
    Vector2 const point{0, 0.5};

    //- Test expected results
    std::array<scalar_t, 3> expectedBaryCoords
    {
        0.25,
        0.25,
        0.5
    };

    //- Evaluate test function
    std::array<scalar_t, 3> baryCoords
    {
        algo::getBarycentricCoordinates(triangle, point)
    };

    //- Assertions
    ASSERT_NEAR(expectedBaryCoords[0], baryCoords[0], TOL);
    ASSERT_NEAR(expectedBaryCoords[1], baryCoords[1], TOL);
    ASSERT_NEAR(expectedBaryCoords[2], baryCoords[2], TOL);
}


TEST(TriangleAlgo, isIntersecting)
{
    //- Test input scenario
    Triangle2 tri1{{{-0.5, 0},   {0.5, 0},   {0,1}}};
    Triangle2 tri2{{{-0.5,-0.5}, {0.5,-0.5}, {0,0.5}}};  // same as tri1 but -0.5 offset in y

    //- Test expected results
    scalar_t expected_area{0.5};

    //-Evaluate test function
    scalar_t const area1 {algo::triangleArea(tri1)};
    scalar_t const area2 {algo::triangleArea(tri2)};
    Vector2 const * const isOverlapping{algo::isOverlapping(tri1, tri2)};

    //- Assertions
    ASSERT_NEAR(expected_area, area1, TOL);
    ASSERT_NEAR(expected_area, area2, TOL);
    EXPECT_TRUE(isOverlapping != nullptr);
}


TEST(TriangleAlgo, areaMomentOfCircle)
{
    constexpr label_t n_vertices{360*2};
    constexpr Vector2 origin{0, 0};
    constexpr scalar_t radius{10.5};
    constexpr auto circle{makeCircle<n_vertices>(radius)};

    constexpr scalar_t r4{radius*radius*radius*radius};

    auto getInertia = [](auto const polygon, auto const centroid) constexpr
    {
        scalar_t inertia{0};
        for (Triangle2 const & t : polygon.triangles)
        {
            scalar_t area{algo::triangleArea(t)};
            inertia += algo::triangleAreaMoment(t, area, centroid);
        }
        return inertia;
    };

    constexpr scalar_t expected_inertia{scalar_t(M_PI) * r4 / scalar_t(2.0)};
    /*constexpr*/ scalar_t calculated_inertia {getInertia(circle, origin)};

    ASSERT_NEAR(expected_inertia, calculated_inertia, (1E-4)*expected_inertia);
}