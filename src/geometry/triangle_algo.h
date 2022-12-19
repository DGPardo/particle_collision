#ifndef TRIANGLE_ALGO_H
#define TRIANGLE_ALGO_H


#include "math/vector_math.h"
#include "segment.h"
#include "triangle.h"

#include <vector>
#include <memory>


template<label_t NVerts> class ConvexPolygon;
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
isPointInsideTriangle(Triangle2 const & tri_1, Vector2 const & pt);


//------------------------------------------------------------------------------

bool
isPointInsidePolygon(TriangleGroup const & g, Vector2 const & pt);

//------------------------------------------------------------------------------

bool
isOverlapping(Triangle2 const & tri_1, Triangle2 const & tri_2);


bool
areOverlapping(TriangleGroup const & group1, TriangleGroup const & group2);

//------------------------------------------------------------------------------

bool
isPointOnASegment(Segment2 const & s, Vector2 const & pt, scalar_t const tol);

}


#endif