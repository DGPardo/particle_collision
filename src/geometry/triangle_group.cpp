#include "triangle_group.h"


TriangleGroup::
TriangleGroup
(
    Vector2 pos0,
    Vector2 vel0
)
  : position{std::move(pos0)}
  , velocity{std::move(vel0)}
  , angular_velocity{0.01}
  , area{0}
  , moment_of_inertia{0}
{
  _triangles.reserve(3*100);  // Reserve for 100 triangles
}


TriangleGroup::
TriangleGroup(TriangleGroup&& other)
  : position{std::move(other.position)}
  , velocity{std::move(other.velocity)}
  , orientation{std::move(other.orientation)}
  , angular_velocity{std::move(other.angular_velocity)}
  , area{std::move(other.area)}
  , moment_of_inertia{std::move(other.moment_of_inertia)}
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
  moment_of_inertia += algo::triangleAreaMoment(coords, area, position);
  _triangles.emplace_back(std::move(coords));
}


std::vector<Triangle2>
TriangleGroup::
getAbsTriangles() const
{
  scalar_t const x0 = position[0];
  scalar_t const y0 = position[1];

  std::vector<Triangle2> triangles(_triangles.size());
  int triangle_id{0};
  for (auto const & t : _triangles)
  {
    Triangle2 & triangle = triangles[triangle_id++];
    int v_id{0}; // vertex id
    for (auto const & v : t)
    {
      triangle[v_id][0] = v[0]*cos(orientation) - v[1]*sin(orientation) + x0;
      triangle[v_id][1] = v[0]*sin(orientation) + v[1]*cos(orientation) + y0;
      v_id++;
    }
  }
  return triangles;
}