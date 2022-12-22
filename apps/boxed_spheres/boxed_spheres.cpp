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

    scalar_t x{-0.8}, y{-0.8};
    for (int i{0}; i < 50; ++i)
    {
        auto circle = makeCircle<12>(std::max(0.01, 0.05*abs(randomScalar())));
        tri_manager.addGroup
        (
            Vector2{x, y},
            randomVector(),
            circle
        );
        if (x < 0.8)
        {
            x += 0.1;
        }
        else
        {
            x = -0.8;
            y += 0.1;
        }
    }

    auto & bdry_manager {BoundariesManager::getSingleton()};

    constexpr Vector2 c1{-0.9, -0.9};
    constexpr Vector2 c2{+0.9, +0.9};
    constexpr Rectangle domain{c1, c2};

    bdry_manager.setBoundary(domain);

    GLFWwindow * const window{render::glInitialize()};
    render::run(window);
}
