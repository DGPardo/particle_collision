#include "math_random.h"


scalar_t 
algo::
randomScalar()
{
    return static_cast<scalar_t>(std::rand()) / static_cast<scalar_t>(RAND_MAX);  // range is [0, 1)
}


scalar_t
algo::
randomScalar(scalar_t min_r, scalar_t max_r)
{
    return min_r + randomScalar() * (max_r - min_r);
}


Vector2
algo::
randomVector(scalar_t min_r, scalar_t max_r)
{
    return {randomScalar(min_r, max_r), randomScalar(min_r, max_r)};
}

