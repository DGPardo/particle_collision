#ifndef GL_MAIN_H_
#define GL_MAIN_H_


#include "GL/glew.h"
#include "GLFW/glfw3.h"


namespace render
{

GLFWwindow * glInitialize();

void run(GLFWwindow * const window);

}

#endif
