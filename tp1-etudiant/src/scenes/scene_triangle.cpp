#include "scene_triangle.h"

#include "vertices_data.h"

#include "utils.h"

SceneTriangle::SceneTriangle(Resources& res)
: Scene(res), m_triangleBuffer(GL_ARRAY_BUFFER), m_triangleDraw(m_triangleVao, 3)
{
    // TODO
    m_triangleBuffer.allocate(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW); //on a change allocate a public

    m_triangleVao.specifyAttribute(m_triangleBuffer, 0, 2, 2 * sizeof(float), 0 * sizeof(float));
}

void SceneTriangle::run(Window& w)
{
    // TODO
    m_resources.basic.use();

    m_triangleDraw.draw();
    m_triangleVao.unbind();
}

