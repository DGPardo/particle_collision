#include "boundary_manager.h"


BoundariesManager &
BoundariesManager::
getSingleton()
{
    static BoundariesManager instance;
    return instance;
}



std::vector<Segment2> const &
BoundariesManager::
getBoundary() const
{
    return _boundary;
}


TriangleGroup const &
BoundariesManager::
getDomain() const
{
    return *(_group.get());
}
