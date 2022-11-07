#ifndef TRIANGLE_ALGO_H
#define TRIANGLE_ALGO_H


#include "math/vector_math.h"
#include "triangle.h"

#include <vector>
#include <memory>


class TriangleGroup;


namespace algo
{

scalar_t
triangleArea(Triangle2 const & verts);

//------------------------------------------------------------------------------

std::array<scalar_t, 3>
getBarycentricCoordinates(Triangle2 const & verts, Vector2 const & point);

//------------------------------------------------------------------------------

bool
isOverlapping(Triangle2 const & tri_1, Triangle2 const & tri_2);


bool
areOverlapping(TriangleGroup const & group1, TriangleGroup const & group2);

//------------------------------------------------------------------------------

}


#endif