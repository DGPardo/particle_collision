#ifndef TRIANGLE_GROUP_H_
#define TRIANGLE_GROUP_H_

#include "triangle.h"
#include "triangle_algo.h"
#include "math/vector_math.h"

#include <array>
#include <vector>


/*----------------------------------------------------------------------------*\
Name:   TriangleGroup
Intent: 
    This object is reponsible for tracking the state (position and velocity)
    of the different particles (triangulated polygons)
    
    Stored coordinates are relative the group position. The methods containing
    the infix "Abs" compute position in global coordinates.

Limitations:
    
\*----------------------------------------------------------------------------*/


class TriangleGroup
{
public:
    //- Constructor

    explicit TriangleGroup
    (
        Vector2 initial_position,
        Vector2 initial_velocity
    );
    TriangleGroup(TriangleGroup&& other);

    // Do not allow empty constructor or copy constructor
    TriangleGroup() = delete;
    TriangleGroup(TriangleGroup const & other) = delete;

    //- member functions
    void addTriangle(Triangle2 coords);
    void addBoundarySegment(Segment2 coords);

    std::vector<Triangle2> const & getTriangles() const;
    std::vector<Triangle2> getAbsTriangles() const;  // Transformed to global coordinates

    std::vector<Segment2> const & getBoundary() const;
    std::vector<Segment2> getAbsBoundary() const;  // Transformed to global coordinates

    //- member attributes
    Vector2 position;
    Vector2 velocity;

    scalar_t orientation;
    scalar_t angular_velocity;

    scalar_t area;
    scalar_t moment_of_inertia;

    scalar_t influence_radius;

private:
    std::vector<Triangle2> _triangles;  // this coordinates are relative to position
    std::vector<Segment2> _boundary;
};


#endif
