#ifndef MATH_RANDOM_H_
#define MATH_RANDOM_H_

#include "vector_math.h"

#include <random>


namespace algo
{

constexpr static int prandom_seed{42};

scalar_t randomScalar();

scalar_t randomScalar(scalar_t min_r, scalar_t max_r);

Vector2 randomVector(scalar_t min_r, scalar_t max_r);  // min and max apply to components, not magnitude

}


#endif
