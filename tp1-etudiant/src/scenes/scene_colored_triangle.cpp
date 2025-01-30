#include "scene_colored_triangle.h"

#include "vertices_data.h"

SceneColoredTriangle::SceneColoredTriangle(Resources& res)
: Scene(res), m_coloredTriangleDraw(m_coloredTriangleVao, 3)
{
    // TODO
    res.coloredTriangleBuffer.allocate(GL_ARRAY_BUFFER, sizeof(colorTriVertices), colorTriVertices, GL_STATIC_DRAW);

    m_coloredTriangleVao.specifyAttribute(res.coloredTriangleBuffer, 0, 2, 5, 0);

    m_coloredTriangleVao.specifyAttribute(res.coloredTriangleBuffer, 1, 3, 5, 2);
}

void SceneColoredTriangle::run(Window& w)
{
    // TODO
    m_resources.color.use();

    m_coloredTriangleDraw.draw();
}

