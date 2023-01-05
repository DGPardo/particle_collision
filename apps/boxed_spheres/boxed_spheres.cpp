#include "geometry/polygon.h"
#include "geometry/triangles_manager.h"
#include "geometry/boundary_manager.h"
#include "math/math_random.h"
#include "rendering/gl_main.h"

#include <ctime>
#include <iostream>



int main(void)
{
    //- Pseudo-random
    std::srand(algo::prandom_seed);

    //- Define Rigid Boundary
    auto & bdry_manager {BoundariesManager::getSingleton()};
    
    constexpr scalar_t width{1.8};
    constexpr scalar_t height{1.8};

    constexpr Vector2 c1{-width/2, -height/2};  // +- 1 are the window borders
    constexpr Vector2 c2{+width/2, +height/2};
    constexpr Rectangle domain{c1, c2};
    
    bdry_manager.setBoundary(domain);
    
    //- Define number of vertices representing a circle
    constexpr label_t nverts{12};

    //- Define particle min and max radius
    scalar_t min_radius{0.01};
    scalar_t max_radius{0.05};

    //- Define initial state: Number of particles, position and velocity
    auto & tri_manager {TrianglesManager::getSingleton()};
    for (int i{0}; i < 25; ++i)
    {
        auto const radius{algo::randomScalar(min_radius, max_radius)};
        auto const circle{makeCircle<nverts>(radius)};

        auto const x{algo::randomScalar(-width/2 + radius, width/2 - radius)};
        auto const y{algo::randomScalar(-height/2 + radius, height/2 - radius)};

        tri_manager.addGroup
        (
            Vector2{x, y},  // Position
            algo::randomVector(-0.5, 0.5), // Velocity components between -0.5 and 0.5 m/s
            circle
        );
    }

    GLFWwindow * const window{render::glInitialize()};
    render::run(window);
}
