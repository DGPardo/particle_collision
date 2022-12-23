#ifndef DRAW_H_
#define DRAW_H_


class GLFWwindow;
class Rectangle;
class TriangleGroup;



namespace render
{

constexpr int window_width {640};
constexpr int window_height {480};

void drawTriangleGroups();

void drawTriangleGroup(TriangleGroup const & group);

void drawBoundary();

void drawQuadTree();

void mouseCallBack(GLFWwindow* window, int button, int action, int mods);

}


#endif

