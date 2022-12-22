#include "draw.h"

#include "geometry/boundary_manager.h"
#include "geometry/polygon.h"
#include "geometry/quad_tree.h"
#include "geometry/triangle_group.h"

#include "physics/physics.h"

#include "rendering/parallel_utils.h"
#include "rendering/gl_traits.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"


#include <mutex>


namespace
{

void
getQuadTreeVertices(QuadTree const * const qt, std::vector<Vector2> & vertices)
{
    if (qt == nullptr) return;

    if (qt->is_subdivided)
    {
        getQuadTreeVertices(qt->north_east.get(), vertices);
        getQuadTreeVertices(qt->north_west.get(), vertices);
        getQuadTreeVertices(qt->south_east.get(), vertices);
        getQuadTreeVertices(qt->south_west.get(), vertices);
        return;
    }

    std::unique_lock lck{render::mtx};
    auto const range_max{qt->boundary.vertices.size() - 1};
    for (label_t i{0}; i != range_max; ++i)  // 3 because there are 4 vertices
    {
        vertices.push_back(qt->boundary.vertices[i]);
        vertices.push_back(qt->boundary.vertices[i+1]);
    }
    vertices.push_back(qt->boundary.vertices.back());
    vertices.push_back(qt->boundary.vertices[0]);
    lck.unlock();
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
    std::unique_lock lck{render::mtx};
    std::vector<Triangle2> const triangles{group.getAbsTriangles()};
    lck.unlock();

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
    std::unique_lock lck(render::mtx);
    Physics & physics{Physics::getSingleton()};
    BoundariesManager & boundaries {BoundariesManager::getSingleton()};
    TrianglesManager & tri_manager{physics.tri_manager};

    std::vector<TriangleGroup> const & groups {tri_manager.getTriangleGroups()};
    QuadTree qt{boundaries.getBoundingBox()};
    for (auto & group : groups)
    {
        qt.insert(QuadTreeNode::make(group.position, group.ptr.get()));
    }
    lck.unlock();

    std::vector<Vector2> vertices; vertices.reserve(100);
    getQuadTreeVertices(&qt, vertices);

    label_t const n_vertices{vertices.size()};

    glBufferData
    (
        GL_ARRAY_BUFFER,
        n_vertices * 2 * sizeof(scalar_t),  // 2 floats per vertex coord
        vertices.data(),
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
    glDrawArrays(GL_LINES, 0, int(n_vertices));
}

