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
    static constexpr ConvexPolygon<4> circle{makeCircle<4>(radius)};

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


TEST(Rectangle, Contains)
{
    //- setup
    static constexpr Vector2 south_west{0,0};
    static constexpr Vector2 north_east{1,1};
    Rectangle rect(south_west, north_east);

    //- test points
    static constexpr Vector2 inside_pt{0.5, 0.5};
    static constexpr Vector2 outside_pt{2, 2};

    ASSERT_TRUE (rect.contains(inside_pt));
    ASSERT_FALSE(rect.contains(outside_pt));
}


TEST(Rectangle, ContainsEdgeTesting)
{
    //- setup
    static constexpr Vector2 south_west{0,0};
    static constexpr Vector2 north_east{1,1};
    Rectangle rect(south_west, north_east);

    //- test points
    static constexpr Vector2 edge_1{0, 0.5};
    static constexpr Vector2 edge_2{1, 0.5};
    static constexpr Vector2 edge_3{0.5, 0};
    static constexpr Vector2 edge_4{0.5, 1};

    ASSERT_FALSE (rect.contains(edge_1));
    ASSERT_FALSE (rect.contains(edge_3));

    ASSERT_TRUE(rect.contains(edge_2));
    ASSERT_TRUE(rect.contains(edge_4));
}


TEST(Rectangle, Overlaps)
{
    //- setup
    static constexpr Vector2 south_west{0,0};
    static constexpr Vector2 north_east{1,1};
    static constexpr Rectangle reference(south_west, north_east);

    {
        //- exact overlap
        Vector2 min{0, 0};
        Vector2 max{1, 1};
        Rectangle other(min, max);
        ASSERT_TRUE(reference.overlaps(other));
        ASSERT_TRUE(other.overlaps(reference));
    }
    {
        //- Completely outside
        Vector2 min{2, 2};
        Vector2 max{3, 5};
        Rectangle other(min, max);
        ASSERT_FALSE(reference.overlaps(other));
        ASSERT_FALSE(other.overlaps(reference));
    }
    {
        //- Partially overlapping
        Vector2 min{0.5, 0.5};
        Vector2 max{2, 2};
        Rectangle other(min, max);
        ASSERT_TRUE(reference.overlaps(other));
        ASSERT_TRUE(other.overlaps(reference));
    }
    {
        //- fully contained
        Vector2 min{0.1, 0.1};
        Vector2 max{0.9, 0.9};
        Rectangle other(min, max);
        ASSERT_TRUE(reference.overlaps(other));
        ASSERT_TRUE(other.overlaps(reference));
    }
}



