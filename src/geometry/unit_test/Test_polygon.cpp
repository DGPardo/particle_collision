#include "gtest/gtest.h"

#include "geometry/polygon.h"

#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*100)


TEST(Polygon, ConvexPolygonConstructor)
{
    //- Test Input scenario
    static constexpr std::array<Vector2, 4> vertices
    {{
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    }};

    //- Test expected results
    static constexpr std::array<Triangle2, 2> expected_triangles
    {{
        {vertices[0], vertices[1], vertices[2]},
        {vertices[0], vertices[2], vertices[3]}
    }};

    //- Evaluate test function
    static constexpr ConvexPolygon<4> quad(vertices);

    //- Assertions
    ASSERT_EQ(expected_triangles.size(), ConvexPolygon<4>::n_triangles);

    for (unsigned tri_id{0}; tri_id != 2; ++tri_id)
    {
        for (unsigned vertex_id{0}; vertex_id != 3; ++vertex_id)
        {
            for (unsigned coord_id{0}; coord_id != 2; ++coord_id)
            {
                scalar_t expected_value{expected_triangles[tri_id][vertex_id][coord_id]};
                scalar_t test_value{quad.triangles[tri_id][vertex_id][coord_id]};
                ASSERT_NEAR(expected_value, test_value, TOL);
            }
        }
    }
}


TEST(Polygon, Circle)
{
    //- Test Input scenario
    static constexpr scalar_t radius = 1;

    //- Test expected results
    static constexpr Vector2 right{1,0};
    static constexpr Vector2 up{0,1};
    static constexpr Vector2 left{-1,0};
    static constexpr Vector2 down{0,-1};
    static constexpr std::array<Triangle2, 2> expected_triangles
    {{
        {right, up, left},
        {right, left, down}
    }};

    //- Evaluate test function
    static constexpr ConvexPolygon<4> circle{makePolygon<4>(radius)};

    //- Assertions
    ASSERT_EQ(expected_triangles.size(), ConvexPolygon<4>::n_triangles);

    for (unsigned tri_id{0}; tri_id != 2; ++tri_id)
    {
        for (unsigned vertex_id{0}; vertex_id != 3; ++vertex_id)
        {
            for (unsigned coord_id{0}; coord_id != 2; ++coord_id)
            {
                scalar_t expected_value{expected_triangles[tri_id][vertex_id][coord_id]};
                scalar_t test_value{circle.triangles[tri_id][vertex_id][coord_id]};
                ASSERT_NEAR(expected_value, test_value, TOL);
            }
        }
    }
}