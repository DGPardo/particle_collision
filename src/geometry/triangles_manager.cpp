#include "triangles_manager.h"


TrianglesManager &
TrianglesManager::
getSingleton()
{
    static TrianglesManager instance;
    return instance;
}


std::vector<TriangleGroup> &
TrianglesManager::
getTriangleGroups()
{
    return _triangleGroups;
}
