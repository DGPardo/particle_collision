#ifndef COORDINATE_TRANSFORMATION_H_
#define COORDINATE_TRANSFORMATION_H_

#include "math/vector_math.h"
#include "triangle_group.h"


namespace algo
{


Vector2 rotatePoint(scalar_t angle_rads, Vector2 const & pt);

Vector2 toRelative(TriangleGroup const & group, Vector2 const & pt);
Vector2 toAbsolute(TriangleGroup const & group, Vector2 const & pt);


}


#endif