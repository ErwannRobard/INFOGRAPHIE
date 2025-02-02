#include "scene_square.h"

#include "vertices_data.h"

SceneSquare::SceneSquare(Resources& res)
: Scene(res), m_squareBuffer(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW), m_squareDraw(m_squareVao, 6)
{
    // TODO
    m_squareBuffer.allocate(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    m_squareVao.specifyAttribute(m_squareBuffer, 0, 2, 5, 0);

    m_squareVao.specifyAttribute(m_squareBuffer, 1, 3, 5, 2);
}

void SceneSquare::run(Window& w)
{
    // TODO
    m_resources.basic.use();

    m_squareDraw.draw();
}

