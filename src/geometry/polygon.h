#ifndef POLYGON_H_
#define POLYGON_H_


#include "triangle.h"
#include "triangle_algo.h"
#include "math/vector_math.h"

#include <math.h>


template<label_t NVerts>
struct ConvexPolygon
{
public:
    static constexpr label_t n_triangles = NVerts - 2;  // Euler Formula

    constexpr ConvexPolygon(std::array<Vector2, NVerts> vertices)
        : triangles{makeTriangles(vertices)}
        , vertices{std::move(vertices)}
    {}

    std::array<Triangle2, n_triangles> const triangles;  // Euler Formula
    std::array<Vector2, NVerts> const vertices;

private:

    static constexpr std::array<Triangle2, n_triangles>
    makeTriangles(std::array<Vector2, NVerts> const & vertices)
    {
        std::array<Triangle2, n_triangles> triangles;
        label_t triangle_id{0};
        for (label_t vertex_id{1}; vertex_id != NVerts - 1; ++vertex_id)
        {
            Triangle2 & triangle = triangles[triangle_id++];
            triangle[0] = vertices[0];
            triangle[1] = vertices[vertex_id];
            triangle[2] = vertices[vertex_id + 1];
        }
        return triangles;
    }
};


template<label_t NVerts>
constexpr ConvexPolygon<NVerts> makePolygon(scalar_t const radius)
{
    scalar_t theta{0};
    scalar_t const delta_theta{2*scalar_t(M_PI)/(NVerts)};
    std::array<Vector2, NVerts> vertices;

    for (label_t v_id{0}; v_id != NVerts; ++v_id)
    {
        vertices[v_id] = {radius * cos(theta), radius * sin(theta)};
        theta += delta_theta;
    }
    return ConvexPolygon<NVerts>(vertices);
}


constexpr ConvexPolygon<4> makeRectangle(Vector2 lwr_left, Vector2 upr_right)
{
    Vector2 const dx {upr_right[0] - lwr_left[0], 0};
    return ConvexPolygon<4>{{lwr_left, lwr_left + dx, upr_right, upr_right - dx}};
}


#endif