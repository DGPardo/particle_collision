#include "gl_main.h"

#include "draw.h"
#include "physics/physics.h"

#include <chrono>
#include <exception>


GLFWwindow *
render::
glInitialize()
{
    GLFWwindow * window;

    /* Initialize the library */
    if (!glfwInit())
    {
        throw std::exception{};
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Particle Collision Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::exception{};
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "glewInit NOT OK" << std::endl;
        throw std::exception();
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
    TrianglesManager & tri_manager{physics.tri_manager};

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        physics.advance();

        for (auto const & triangle_group : tri_manager.getTriangleGroups())
        {
            render::drawTriangleGroup(triangle_group);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
}
