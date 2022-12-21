#include "gtest/gtest.h"

#include "geometry/polygon.h"
#include "geometry/quad_tree.h"


#include <limits>
#define TOL (std::numeric_limits<scalar_t>::epsilon()*100)


TEST(QuadTree, Insertion)
{
    constexpr Vector2 min{0,0};
    constexpr Vector2 max{1,1};
    constexpr Rectangle boundary{min, max};
    constexpr label_t capacity{4};

    QuadTree qt(boundary, capacity);

    void* data{nullptr};

    Vector2 position{.5, .5};
    qt.insert(QuadTreeNode::make(position, data));

    auto const found {qt.query(Rectangle(Vector2{0, 0}, Vector2{1, 1}))};
    ASSERT_EQ(found.size(), label_t(1));
    ASSERT_NEAR(found.back().position[0], position[0], TOL);
    ASSERT_NEAR(found.back().position[1], position[1], TOL);
}


TEST(QuadTree, InsertionSubdivide)
{
    constexpr Vector2 min{0,0};
    constexpr Vector2 max{1,1};
    constexpr Rectangle boundary{min, max};
    constexpr label_t capacity{4};

    QuadTree qt(boundary, capacity);

    void* data{nullptr};

    constexpr Vector2 point_cloud[7] =
    {
        {0.1, 0.1},
        {0.2, 0.2},
        {0.3, 0.3},
        {0.4, 0.4},
        {0.5, 0.5},
        {0.6, 0.6},
        {0.7, 0.7},
    };

    for (label_t i{0}; i != 7; ++i)
    {
        qt.insert(QuadTreeNode::make(point_cloud[i], data));
    }

    auto const found {qt.query(Rectangle(Vector2{0, 0}, Vector2{1, 1}))};
    ASSERT_EQ(found.size(), label_t(7));
}
