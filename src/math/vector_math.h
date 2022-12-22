#ifndef VECTOR_MATH_H_
#define VECTOR_MATH_H_

#include <array>
#include <iostream>
#include <limits>
#include <math.h>


#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*100)


static constexpr scalar_t SCALAR_MAX {std::numeric_limits<scalar_t>::max()};
static constexpr scalar_t SCALAR_MIN {std::numeric_limits<scalar_t>::lowest()};


template<unsigned N>
using ScalarArray = std::array<scalar_t, N>;

using Vector2 = ScalarArray<2>;


//------------------------------------------------------------------------------
//- non-member functions


std::ostream & operator<<(std::ostream & os, Vector2 const & vector);


//------------------------------------------------------------------------------
//- non-member constexpr functions


constexpr Vector2 operator+(Vector2 const & v1, Vector2 const & v2)
{
    return {v1[0] + v2[0], v1[1] + v2[1]};
}


constexpr Vector2 operator-(Vector2 const & v1, Vector2 const & v2)
{
    return {v1[0] - v2[0], v1[1] - v2[1]};
}


constexpr Vector2 operator/(Vector2 const & v1, scalar_t scalar)
{
    return {v1[0]/scalar, v1[1]/scalar};
}


constexpr Vector2 operator*(Vector2 const & v1, scalar_t scalar)
{
    return {v1[0]*scalar, v1[1]*scalar};
}


constexpr Vector2 operator*(scalar_t scalar, Vector2 const & v1)
{
    return v1 * scalar;
}


constexpr Vector2 & operator+=(Vector2 & v1, Vector2 const & v2)
{
    v1[0] += v2[0];
    v1[1] += v2[1];
    return v1;
}


constexpr Vector2 & operator-=(Vector2 & v1, Vector2 const & v2)
{
    v1[0] -= v2[0];
    v1[1] -= v2[1];
    return v1;
}


constexpr Vector2 & operator/=(Vector2 & v1, scalar_t scalar)
{
    v1[0] /= scalar;
    v1[1] /= scalar;
    return v1;
}


constexpr Vector2 & operator*=(Vector2 & v1, scalar_t scalar)
{
    v1[0] *= scalar;
    v1[1] *= scalar;
    return v1;
}


constexpr scalar_t dot(Vector2 const & v1, Vector2 const & v2)
{
    return (v1[0] * v2[0]) + (v1[1] * v2[1]);
}


constexpr scalar_t magSqr(Vector2 const & vector)
{
    return dot(vector, vector);
}


constexpr scalar_t mag(Vector2 const & vector)
{
    return sqrt(magSqr(vector));
}


constexpr Vector2 unitVector(Vector2 const & vector)
{
    return vector / (mag(vector) + TOL);
}


#endif
