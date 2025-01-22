#include "scene_square.h"

#include "vertices_data.h"

SceneSquare::SceneSquare(Resources& res)
: Scene(res), m_squareBuffer(GL_ARRAY_BUFFER), m_squareDraw(m_squareVao, 6)
{
    // TODO
    m_squareBuffer.allocate(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    m_squareVao.specifyAttribute(m_squareBuffer, 0, 2, 5 * sizeof(float), 0 * sizeof(float));

    m_squareVao.specifyAttribute(m_squareBuffer, 1, 3, 5 * sizeof(float), 2 * sizeof(float));
}

void SceneSquare::run(Window& w)
{
    // TODO
    m_resources.basic.use();

    m_squareDraw.draw();
}

