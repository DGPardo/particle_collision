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

    constexpr auto circle{makeCircle<12>(0.02)};

    for (label_t i{0}; i != 20; ++i)    
    {
        tri_manager.addGroup
        (
            randomVector(),
            randomVector(),
            circle
        );
    }

    auto & bdry_manager {BoundariesManager::getSingleton()};
    constexpr auto domain{makeCircle<180>(1)};
    bdry_manager.setBoundary(domain);

    GLFWwindow * const window{render::glInitialize()};
    render::run(window);
}
