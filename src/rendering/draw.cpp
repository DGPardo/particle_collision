#include "draw.h"

#include "geometry/boundary_manager.h"
#include "geometry/polygon.h"
#include "geometry/quad_tree.h"
#include "geometry/triangle_group.h"
#include "gl_traits.h"
#include "physics/physics.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"


namespace
{

void
drawQuadTreeHelper(QuadTree const * const qt)
{
    if (qt == nullptr) return;

    label_t const n_vertices{qt->boundary.vertices.size()};

    glBufferData
    (
        GL_ARRAY_BUFFER,
        n_vertices * 2 * sizeof(scalar_t),  // 2 floats per vertex coord
        qt->boundary.vertices.begin()->data(),
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
    glDrawArrays(GL_LINE_LOOP, 0, int(n_vertices));

    drawQuadTreeHelper(qt->north_east.get());
    drawQuadTreeHelper(qt->north_west.get());
    drawQuadTreeHelper(qt->south_east.get());
    drawQuadTreeHelper(qt->south_west.get());
}

}


void
render::
drawTriangleGroups() //(std::mutex & mtx)
{
    Physics & physics{Physics::getSingleton()};
    TrianglesManager & tri_manager{physics.tri_manager};

    std::vector<TriangleGroup> const & groups {tri_manager.getTriangleGroups()};

    for (auto const & triangle_group : groups)
    {
        render::drawTriangleGroup(triangle_group);
    }
}


void
render::
drawTriangleGroup(TriangleGroup const & group)
{
    std::vector<Triangle2> const triangles{group.getAbsTriangles()};
    label_t const n_vertices{triangles.size()*3};

    glBufferData
    (
        GL_ARRAY_BUFFER,
        n_vertices * 2 * sizeof(scalar_t),  // 2 floats per vertex coord
        triangles.data(),
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

    glDrawArrays(GL_TRIANGLES, 0, int(n_vertices));
}


void
render::
drawBoundary() //(std::mutex & mtx)
{
    BoundariesManager & boundaries {BoundariesManager::getSingleton()};
    std::vector<Segment2> const & boundary {boundaries.getBoundary()};

    label_t const n_vertices{boundary.size() * 2};
    glBufferData
    (
        GL_ARRAY_BUFFER,
        n_vertices * 2 * sizeof(scalar_t),  // 2 floats per vertex coord
        boundary.begin()->data(),
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


void
render::
drawQuadTree()
{
    Physics & physics{Physics::getSingleton()};
    BoundariesManager & boundaries {BoundariesManager::getSingleton()};
    TrianglesManager & tri_manager{physics.tri_manager};

    std::vector<TriangleGroup> const & groups {tri_manager.getTriangleGroups()};
    QuadTree qt{boundaries.getBoundingBox()};
    for (auto & group : groups)
    {
        qt.insert(QuadTreeNode::make(group.position, group.ptr.get()));
    }

    drawQuadTreeHelper(&qt);
}

