#include "bounding_box.h"
#include "triangle_group.h"

Rectangle
algo::
getBoundingBox(TriangleGroup const & group, scalar_t offset)
{
    return getBoundingBox(group.getAbsBoundary(), offset);
}


Rectangle
algo::
getBoundingBox(std::vector<Segment2> const & boundary, scalar_t offset)
{
    scalar_t xmin{SCALAR_MAX}, xmax{SCALAR_MIN}, ymin{SCALAR_MAX}, ymax{SCALAR_MIN};
    for (Segment2 const & s : boundary)
    {
        for (Vector2 const coord : s)
        {
            if      (coord[0] < xmin) { xmin = coord[0]; }
            else if (coord[0] > xmax) { xmax = coord[0]; }

            if      (coord[1] < ymin) { ymin = coord[1]; }
            else if (coord[1] > ymax) { ymax = coord[1]; }
        }
    }
    xmin -= offset;
    ymin -= offset;

    xmax += offset;
    ymax += offset;
    return Rectangle(Vector2{xmin, ymin}, Vector2{xmax, ymax});
}