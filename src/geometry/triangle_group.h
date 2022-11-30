#ifndef TRIANGLE_GROUP_H_
#define TRIANGLE_GROUP_H_

#include "triangle.h"
#include "triangle_algo.h"
#include "math/vector_math.h"

#include <array>
#include <vector>


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
    std::vector<Triangle2> const & getTriangles() const;
    void addTriangle(Triangle2 coords);

    //- member attributes
    Vector2 position;
    Vector2 velocity;

    scalar_t orientation;
    scalar_t orientation_velocity;

    scalar_t area;
    scalar_t moment_of_inertia;

private:
    std::vector<Triangle2> _triangles;  // this coordinates are relative to position
};


#endif

