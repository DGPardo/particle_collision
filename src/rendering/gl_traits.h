#include "GL/glew.h"
#include "GLFW/glfw3.h"


namespace render
{

template<class SCALAR>
constexpr auto gl_scalar_v = 0;

template<>
constexpr auto gl_scalar_v<float> = GL_FLOAT;

template<>
constexpr auto gl_scalar_v<double> = GL_DOUBLE;

}
