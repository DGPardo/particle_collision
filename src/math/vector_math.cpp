#include "vector_math.h"


std::ostream & operator<<(std::ostream & os, Vector2 const & vector)
{
    return os << "(" << vector[0] << ", " << vector[1] << ")" << std::endl;
}


