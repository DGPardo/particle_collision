#include "triangle_group.h"


TriangleGroup::
TriangleGroup
(
    Vector2 pos0,
    Vector2 vel0
)
  : position{std::move(pos0)}
  , velocity{std::move(vel0)}
  , area{0}
{
  _triangles.reserve(3*100);  // Reserve for 100 triangles
}


TriangleGroup::
TriangleGroup(TriangleGroup&& other)
  : position{std::move(other.position)}
  , velocity{std::move(other.velocity)}
  , area{std::move(other.area)}
  , _triangles{std::move(other._triangles)}
{}


std::vector<Triangle2> const &
TriangleGroup::
getTriangles() const
{
  return _triangles;
}


void
TriangleGroup::
addTriangle(Triangle2 coords)
{
  area += algo::triangleArea(coords);
  _triangles.emplace_back(std::move(coords));
}