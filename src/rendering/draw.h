#ifndef DRAW_H_
#define DRAW_H_


class Rectangle;
class TriangleGroup;


namespace render
{

void drawTriangleGroups();

void drawTriangleGroup(TriangleGroup const & group);

void drawBoundary();

void drawQuadTree();

}


#endif

