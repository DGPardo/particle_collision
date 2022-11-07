#include "geometry/polygon.h"
#include "geometry/triangles_manager.h"
#include "rendering/gl_main.h"

#include <iostream>

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
    TrianglesManager & tri_manager {TrianglesManager::getSingleton()};
    for (int obj_id{0}; obj_id < 2; ++obj_id)
    {
        scalar_t const radius{0.05};
        Circle<12> circle{radius};
        Vector2 const initial_position{randomVector()};
        Vector2 const initial_velocity{randomVector()};
        tri_manager.addGroup<circle.n_triangles>
        (
            initial_position,
            initial_velocity,
            circle.triangles
        );
    }

    GLFWwindow * const window{render::glInitialize()};
    render::run(window);
}
