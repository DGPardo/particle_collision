#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include "polygon.h"
#include "triangle_group.h"

#include <limits>


namespace algo
{

template<label_t N>
Rectangle
getBoundingBox(ConvexPolygon<N> const & polygon, /*unsigned*/ scalar_t offset=0)
{
    scalar_t xmin{SCALAR_MAX}, xmax{SCALAR_MIN}, ymin{SCALAR_MAX}, ymax{SCALAR_MIN};
    for (Vector2 const & coord : polygon.vertices)
    {
        if      (coord[0] < xmin) { xmin = coord[0]; }
        else if (coord[0] > xmax) { xmax = coord[0]; }

        if      (coord[1] < ymin) { ymin = coord[1]; }
        else if (coord[1] > ymax) { ymax = coord[1]; }
    }
    xmin -= offset;
    ymin -= offset;

    xmax += offset;
    ymax += offset;

    return Rectangle(Vector2{xmin, ymin}, Vector2{xmax, ymax});
}


Rectangle
getBoundingBox(TriangleGroup const & group, /*unsigned*/ scalar_t offset=0);


Rectangle
getBoundingBox(std::vector<Segment2> const & boundary, /*unsigned*/ scalar_t offset=0);


}


#endif