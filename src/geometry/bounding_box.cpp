#include "bounding_box.h"
#include "triangle_group.h"

Rectangle
algo::
getBoundingBox(TriangleGroup const & group, scalar_t offset)
{
    Vector2 const diagonal{group.influence_radius + offset, group.influence_radius + offset};
    auto min{group.position - diagonal};
    auto max{group.position + diagonal};
    return Rectangle(std::move(min), std::move(max));
}


Rectangle
algo::
getBoundingBox(std::vector<Segment2> const & boundary, scalar_t offset)
{
    scalar_t xmin, xmax, ymin, ymax;
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
