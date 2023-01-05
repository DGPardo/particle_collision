#ifndef POLYGON_H_
#define POLYGON_H_


#include "triangle.h"
#include "triangle_algo.h"
#include "math/vector_math.h"

#include <math.h>


/*----------------------------------------------------------------------------*\
Name:   ConvexPolygon
Intent: 
    Generate a convex polygon of a given N vertices. The polygon is triangulated
    on construction time by using a "fan" algorithm.

    The object stores the vertices as well as the triangulation information.
    Vertices are oriented counter-clockwise, therefore defining at the same time
    the boundary information.

Limitations:
    
\*----------------------------------------------------------------------------*/


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


struct Rectangle
    : public ConvexPolygon<4>
{
    constexpr Rectangle(Vector2 const lwr_left, Vector2 const upr_right)
        : ConvexPolygon<4>
        {{
            lwr_left,
            lwr_left + Vector2{upr_right[0] - lwr_left[0], 0},
            upr_right,
            upr_right - Vector2{upr_right[0] - lwr_left[0], 0}
        }}
        , center{0.5*(lwr_left + upr_right)}
        , width{upr_right[0] - lwr_left[0]}
        , height{upr_right[1] - lwr_left[1]}
    {}

    Vector2 const center;
    scalar_t const width;
    scalar_t const height;

    constexpr bool contains(Vector2 const pt) const
    {
        auto isNotContained = [&]() -> bool
        {
            auto const xmin {vertices[0][0]};  // lwr_left  @ x
            auto const xmax {vertices[1][0]};  // lwr_right @ x
            auto const ymin {vertices[0][1]};  // lwr_left  @ y
            auto const ymax {vertices[2][1]};  // upr_right @ y

            return (pt[0] <= xmin)
                 | (pt[0] >  xmax)
                 | (pt[1] <= ymin)
                 | (pt[1] >  ymax);
        };
        return !isNotContained();
    }

    constexpr bool overlaps(Rectangle const & other) const
    {
        auto isNotOverlapping = [&]() -> bool
        {
            auto const xmin {vertices[0][0]};  // lwr_left  @ x
            auto const xmax {vertices[1][0]};  // lwr_right @ x
            auto const ymin {vertices[0][1]};  // lwr_left  @ y
            auto const ymax {vertices[2][1]};  // upr_right @ y

            auto const o_xmin {other.vertices[0][0]};
            auto const o_xmax {other.vertices[1][0]};
            auto const o_ymin {other.vertices[0][1]};
            auto const o_ymax {other.vertices[2][1]};

            return xmin > o_xmax || xmax < o_xmin ||
                   ymax < o_ymin || ymin > o_ymax;
        };
        return !isNotOverlapping();
    }

};


template<label_t NVerts>
constexpr ConvexPolygon<NVerts> makeCircle(scalar_t const radius)
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


#endif