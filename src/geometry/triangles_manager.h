#ifndef TRIANGLES_MANAGER_H_
#define TRIANGLES_MANAGER_H_


#include "triangle.h"
#include "triangle_group.h"
#include <vector>


class TrianglesManager
{
public:
    static TrianglesManager & getSingleton();

    template<label_t n>
    void addGroup
    (
        Vector2 position,
        Vector2 velocity,
        std::array<Triangle2, n> const & triangles
    );

    void addInGroup(label_t const gid, Triangle2 tri_verts);

    std::vector<TriangleGroup> & getTriangleGroups();

private:
    TrianglesManager() = default;
    std::vector<TriangleGroup> _triangleGroups;
    label_t _last_id;
};


//------------------------------------------------------------------------------
//- Template function Implementation


template<label_t n>
void
TrianglesManager::
addGroup
(
    Vector2 position,
    Vector2 velocity,
    std::array<Triangle2, n> const & triangles
)
{
    TriangleGroup & group
    {
        _triangleGroups.emplace_back
        (
            std::move(position),
            std::move(velocity)
        )
    };
    for (auto const & triangle : triangles)
    {
        group.addTriangle(triangle);
    }
}


#endif

