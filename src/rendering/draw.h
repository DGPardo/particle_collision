#ifndef DRAW_H_
#define DRAW_H_

#include <mutex>

class TriangleGroup;


namespace render
{

void drawTriangleGroups();

void drawTriangleGroup(TriangleGroup const & group);

void drawBoundary();

}


#endif