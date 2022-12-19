#include "gl_main.h"

#include "draw.h"
#include "physics/physics.h"

#include <chrono>
#include <exception>

#include <thread>
#include <mutex>


GLFWwindow *
render::
glInitialize()
{
    GLFWwindow * window;

    /* Initialize the library */
    if (!glfwInit())
    {
        throw std::runtime_error{"Could not initialize glfw"};
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Particle Collision Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error{"Could not create gflw window"};
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error{"Could not initialize glew"};
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    // TODO Abstract buffers
    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);

    return window;
}


void
render::
run(GLFWwindow * const window)
{
    Physics & physics{Physics::getSingleton()};

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        physics.advance();
        render::drawTriangleGroups();
        render::drawBoundary();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
}
