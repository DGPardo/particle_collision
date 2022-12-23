#include "gl_main.h"

#include "draw.h"
#include "physics/physics.h"
#include "rendering/parallel_utils.h"

#include <chrono>
#include <exception>
#include <mutex>
#include <thread>


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
    window = glfwCreateWindow
    (
        render::window_width,
        render::window_height,
        "Particle Collision Simulation",
        NULL,
        NULL
    );
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

    glfwSetMouseButtonCallback(window, render::mouseCallBack);

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

    auto continuousUpdate =[&](std::stop_token stoken)
    {
        while (!stoken.stop_requested())
        {
            std::lock_guard<std::mutex> lck(render::mtx);
            physics.advance();
        }
    };
    std::jthread phyx{continuousUpdate};

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        render::drawQuadTree();
        render::drawBoundary();
        render::drawTriangleGroups();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    phyx.request_stop();
    phyx.join();

}