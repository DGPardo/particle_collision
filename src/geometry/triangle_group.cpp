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
  , ptr{std::make_unique<TriangleGroup *>(this)}
{
  _triangles.reserve(3*100);  // Reserve for 100 triangles
  _boundary.reserve(100);
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
  , ptr{std::move(other.ptr)}
  , _triangles{std::move(other._triangles)}
  , _boundary{std::move(other._boundary)}
{
  *ptr = this;
}


void
TriangleGroup::
addTriangle(Triangle2 coords)
{
  area += algo::triangleArea(coords);
  moment_of_inertia += algo::triangleAreaMoment(coords, area, Vector2{0, 0});
  
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
    for (auto & vertex : triangle)
    {
      vertex = algo::toAbsolute(*this, vertex);
    }
  }
  return triangles;
}


std::vector<Segment2> const &
TriangleGroup::
getBoundary() const
{
  return _boundary;
}
