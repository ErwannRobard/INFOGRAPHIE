#include "scene_colored_square.h"

#include "vertices_data.h"

SceneColoredSquare::SceneColoredSquare(Resources& res)
: Scene(res), m_coloredSquareBuffer(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW), m_coloredSquareDraw(m_coloredSquareVao, 6)
{
    // TODO
    m_coloredSquareBuffer.allocate(GL_ARRAY_BUFFER, sizeof(colorSquareVertices), colorSquareVertices, GL_STATIC_DRAW);

    m_coloredSquareVao.specifyAttribute(m_coloredSquareBuffer, 0, 2, 5, 0);

    m_coloredSquareVao.specifyAttribute(m_coloredSquareBuffer, 1, 3, 5, 2);
}

void SceneColoredSquare::run(Window& w)
{
    // TODO
    m_resources.color.use();

    m_coloredSquareDraw.draw();
}

