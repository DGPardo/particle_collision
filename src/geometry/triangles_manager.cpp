#include "triangles_manager.h"


TrianglesManager &
TrianglesManager::
getSingleton()
{
    static TrianglesManager instance;
    return instance;
}


void
TrianglesManager::
addInGroup(label_t const gid, Triangle2 tri_verts)
{
    _triangleGroups[gid].addTriangle(std::move(tri_verts));
}


std::vector<TriangleGroup> &
TrianglesManager::
getTriangleGroups()
{
    return _triangleGroups;
}
