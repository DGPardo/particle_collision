#include "triangle_group.h"
#include "coordinate_transformation.h"


TriangleGroup::
TriangleGroup
(
    Vector2 pos0,
    Vector2 vel0
)
  : position{std::move(pos0)}
  , velocity{std::move(vel0)}
  , orientation{0}
  , angular_velocity{0}
  , area{0}
  , moment_of_inertia{0}
  , influence_radius{0}
{
  _triangles.reserve(3*20);  // Reserve for 20 triangles
  _boundary.reserve(20);
}


TriangleGroup::
TriangleGroup(TriangleGroup&& other)
  : position{std::move(other.position)}
  , velocity{std::move(other.velocity)}
  , orientation{std::move(other.orientation)}
  , angular_velocity{std::move(other.angular_velocity)}
  , area{std::move(other.area)}
  , moment_of_inertia{std::move(other.moment_of_inertia)}
  , influence_radius{std::move(other.influence_radius)}
  , _triangles{std::move(other._triangles)}
  , _boundary{std::move(other._boundary)}
{
}


void
TriangleGroup::
addTriangle(Triangle2 coords)
{
  area += algo::triangleArea(coords);
  for (Vector2 const & coord : coords)
  {
    influence_radius = std::max(influence_radius, mag(coord));
  }

  _triangles.emplace_back(std::move(coords));

}


void
TriangleGroup::
addBoundarySegment(Segment2 coords)
{
  _boundary.emplace_back(std::move(coords));
}


std::vector<Triangle2> const &
TriangleGroup::
getTriangles() const
{
  return _triangles;
}


std::vector<Triangle2>
TriangleGroup::
getAbsTriangles() const
{
  std::vector<Triangle2> triangles{_triangles};
  for (auto & triangle : triangles)
  {
    triangle[0] = algo::toAbsolute(*this, triangle[0]);
    triangle[1] = algo::toAbsolute(*this, triangle[1]);
    triangle[2] = algo::toAbsolute(*this, triangle[2]);
  }
  return triangles;
}


std::vector<Segment2>
TriangleGroup::
getAbsBoundary() const
{
  std::vector<Segment2> segment2{_boundary};
  for (auto & s : segment2)
  {
    s[0] = algo::toAbsolute(*this, s[0]);
    s[1] = algo::toAbsolute(*this, s[1]);
  }
  return segment2;
}


std::vector<Segment2> const &
TriangleGroup::
getBoundary() const
{
  return _boundary;
}