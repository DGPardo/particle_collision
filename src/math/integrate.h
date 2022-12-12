#ifndef INTEGRATE_H_
#define INTEGRATE_H_

#include <math.h>

#include <limits>


namespace algo
{


constexpr scalar_t INTEGRAL_TOLERANCE = std::numeric_limits<scalar_t>::epsilon()*100;
constexpr label_t MAX_ITER = 16;  // there will be 2^16 divisions


constexpr scalar_t
integral1(auto f, scalar_t const x1, scalar_t const x2, label_t n_div = 4)
{
    auto linearIntegral = [&]()
    {
        scalar_t const dx = (x2 - x1) / static_cast<scalar_t>(n_div);
        scalar_t x{x1};
        scalar_t integral{0};
        for (label_t step{0}; step < n_div; ++step)
        {
            //- Simpson's 1/3 rule
            integral += dx/6.f * (f(x) + 4*f(x + dx/2.f) + f(x + dx));
            x += dx;
        }
        return integral;
    };

    scalar_t prev_integral{linearIntegral()};
    scalar_t new_integral = prev_integral + 2*algo::INTEGRAL_TOLERANCE;

    label_t n_iterations{1};

    while
    (
        (n_iterations < algo::MAX_ITER)
      & (abs(new_integral - prev_integral) > algo::INTEGRAL_TOLERANCE)
    )
    {
        prev_integral = new_integral;
        n_div *= 2;
        new_integral = linearIntegral();
    }
    return new_integral;
}


}


#endif
