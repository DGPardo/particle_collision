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
    
    //- Launch Renderer and start physics update
    GLFWwindow * const window{render::glInitialize()};
    render::run(window);
}
