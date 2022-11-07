#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "geometry/triangles_manager.h"
#include <chrono>


class Physics
{
public:
    static Physics & getSingleton();
    void advance();
    void advanceGroup(TriangleGroup & tri_group, scalar_t const dt) const;

    std::chrono::high_resolution_clock::time_point const start_time;
    std::chrono::high_resolution_clock::time_point last_time;
    TrianglesManager & tri_manager;

private:
    Physics(std::chrono::high_resolution_clock::time_point const & now);

};


#endif

