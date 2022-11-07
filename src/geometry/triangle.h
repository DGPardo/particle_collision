#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "math/vector_math.h"

using Triangle2 = std::array<Vector2, 3>;


//------------------------------------------------------------------------------


constexpr Triangle2 operator+(Triangle2 const & lhs, Triangle2 const & rhs)
{
    return
    {
        lhs[0] + rhs[0],
        lhs[1] + rhs[1],
        lhs[2] + rhs[2]
    };
}


constexpr Triangle2 operator+(Triangle2 const & lhs, Vector2 const & rhs)
{
    return
    {
        lhs[0] + rhs,
        lhs[1] + rhs,
        lhs[2] + rhs
    };
}


constexpr Triangle2 operator-(Triangle2 const & lhs, Triangle2 const & rhs)
{
    return
    {
        lhs[0] - rhs[0],
        lhs[1] - rhs[1],
        lhs[2] - rhs[2]
    };
}


constexpr Triangle2 operator-(Triangle2 const & lhs, Vector2 const & rhs)
{
    return
    {
        lhs[0] - rhs,
        lhs[1] - rhs,
        lhs[2] - rhs
    };
}


#endif
