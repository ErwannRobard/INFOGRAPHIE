#include "scene_shared_vao.h"

#include "vertices_data.h"

SceneSharedVao::SceneSharedVao(Resources& res)
: Scene(res)
, m_coloredTriangleSharedDraw(m_sharedVao, 3) //Triangle 3 vertices with drawArray
, m_coloredSquareSharedDraw(m_sharedVao, 6, GL_UNSIGNED_BYTE) //square 6 indice
{
    // TODOx
}

void SceneSharedVao::run(Window& w)
{
    // Exceptionnellement, cette méthode n'est pas utilisée
}
    
void SceneSharedVao::runTriangle()
{
    // TODO
    //USE COLOR SHADER EXPLICITELY OR CUBE SCENE TRANSFORMATIONS ROTATES VAOs
    m_resources.color.use();
    //bind buffer only before drawing
    m_sharedVao.bind();
    //coloredTriangle attributes
    m_sharedVao.specifyAttribute(m_resources.coloredTriangleBuffer, 0, 2, 5, 0);
    m_sharedVao.specifyAttribute(m_resources.coloredTriangleBuffer, 1, 3, 5, 2);

    //unbind EBO from active shared VAO, avoid unwanted previous configurations from scene swtich
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_sharedVao.unbind();
    m_coloredTriangleSharedDraw.draw();
}

void SceneSharedVao::runSquare()
{
    // TODO
    //USE COLOR SHADER EXPLICITELY OR CUBE SCENE TRANSFORMATIONS ROTATES VAOs
    m_resources.color.use();
    m_sharedVao.bind();

    m_sharedVao.specifyAttribute(m_resources.coloredSquareReduceBuffer, 0, 2, 5, 0);
    m_sharedVao.specifyAttribute(m_resources.coloredSquareReduceBuffer, 1, 3, 5, 2);
    //EBO for square
    m_resources.coloredSquareReduceIndicesBuffer.bind();
    m_sharedVao.unbind();

    m_coloredSquareSharedDraw.draw();

}


