#include "draw.h"

#include "geometry/boundary_manager.h"
#include "geometry/triangle_group.h"
#include "gl_traits.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"


void
render::
drawTriangleGroup(TriangleGroup const & group)
{
    std::vector<Triangle2> const triangles{group.getAbsTriangles()};
    label_t const n_vertices{triangles.size()*3};

    std::vector<Vector2> vertexAbsPosition(n_vertices);
    label_t vertex_id{0};
    for (Triangle2 const & triangle : triangles)
    {
        vertexAbsPosition[vertex_id++] = triangle[0];
        vertexAbsPosition[vertex_id++] = triangle[1];
        vertexAbsPosition[vertex_id++] = triangle[2];
    }

    glBufferData
    (
        GL_ARRAY_BUFFER,
        n_vertices * 2 * sizeof(scalar_t),  // 2 floats per vertex coord
        vertexAbsPosition.data(),
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer
    (
        /*index*/      0,
        /*size*/       2,  // 2 floats per coordinate
        /*type*/       render::gl_scalar_v<scalar_t>,
        /*normalized*/ GL_FALSE,
        /*stride*/     2 * sizeof(scalar_t),
        /*offset*/     0
    );

    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, int(n_vertices));
}


void
render::
drawBoundary()
{
    BoundariesManager & boundaries {BoundariesManager::getSingleton()};
    label_t const n_vertices{boundaries.getBoundary().size() * 2};
    glBufferData
    (
        GL_ARRAY_BUFFER,
        n_vertices * 2 * sizeof(scalar_t),  // 2 floats per vertex coord
        boundaries.getBoundary().begin()->data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer
    (
        /*index*/      0,
        /*size*/       2,  // 2 floats per coordinate
        /*type*/       render::gl_scalar_v<scalar_t>,
        /*normalized*/ GL_FALSE,
        /*stride*/     2 * sizeof(scalar_t),
        /*offset*/     0
    );

    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINES, 0, int(n_vertices));
}
