#include "gtest/gtest.h"

#include "math/integrate.h"

#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*500)


TEST(Integrate, Integrate1D)
{
    auto f = []([[maybe_unused]] scalar_t const x)
    {
        return 1;
    };

    scalar_t const expected_result {5};
    scalar_t const result {algo::integral1(f, 5, 10)};

    ASSERT_NEAR(result, expected_result, TOL);
}


TEST(Integrate, Integrate1D_complicatedFunction)
{
    auto f = []([[maybe_unused]] scalar_t const x)
    {
        return sin(x)*sin(x);
    };

    scalar_t expected_result {M_PI/2};
    scalar_t result {algo::integral1(f, 0, M_PI)};

    ASSERT_NEAR(result, expected_result, TOL);

    auto g = []([[maybe_unused]] scalar_t const x)
    {
        return x*sin(x*x)*sin(x*x);
    };

    scalar_t const piSqr = M_PI * M_PI;
    expected_result = 0.25*(piSqr - sin(piSqr)*cos(piSqr));
    result = algo::integral1(g, 0, M_PI);

    ASSERT_NEAR(result, expected_result, TOL);
}


TEST(Integrate, Integrate1D_Constexpr)
{
    auto f = []([[maybe_unused]] scalar_t const x)
    {
        return sin(x)*sin(x);
    };

    scalar_t expected_result {M_PI/2};
    static constexpr scalar_t result {algo::integral1(f, 0, M_PI)};

    ASSERT_NEAR(result, expected_result, TOL);
}

