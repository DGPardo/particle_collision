#include "geometry/polygon.h"
#include "geometry/triangles_manager.h"
#include "geometry/boundary_manager.h"
#include "rendering/gl_main.h"

#include <random>
#include <ctime>


static int seed{42};


scalar_t randomScalar()
{
    std::srand(seed++);
    auto const r1 {scalar_t(std::rand())};
    auto const r2 {scalar_t(std::rand())};
    auto const range(scalar_t(RAND_MAX));
    return r1/range - r2/range;
}


Vector2 randomVector()
{
    return {randomScalar(), randomScalar()};
}


int main(void)
{
    auto & tri_manager {TrianglesManager::getSingleton()};

    constexpr Vector2 c1{-0.1, -0.02};
    constexpr Vector2 c2{0.1, 0.02};
    constexpr auto bar_h{makeRectangle(c1, c2)};
    
    constexpr Vector2 c3{-0.05, -0.05};
    constexpr Vector2 c4{0.05, 0.05};
    constexpr auto bar_v{makeRectangle(c3, c4)};
    
    tri_manager.addGroup
    (
        Vector2{0, 0},
        Vector2{0, 0},
        bar_h
    );

    tri_manager.addGroup
    (
        Vector2{0, -0.3},
        Vector2{0, 0.1},
        bar_v
    );

    constexpr auto circle{makeCircle<12>(0.04)};

    tri_manager.addGroup
    (
        Vector2{0.05, 0.3},
        Vector2{0, 3},
        circle
    );

    tri_manager.addGroup
    (
        Vector2{-0.05, -0.3},
        Vector2{3, 0},
        circle
    );


    auto & bdry_manager {BoundariesManager::getSingleton()};
    constexpr Vector2 ll{-0.8, -0.8};
    constexpr Vector2 ur{0.8, 0.8};
    constexpr auto domain{makeRectangle(ll, ur)};
    bdry_manager.setBoundary(domain);

    GLFWwindow * const window{render::glInitialize()};
    render::run(window);
}
