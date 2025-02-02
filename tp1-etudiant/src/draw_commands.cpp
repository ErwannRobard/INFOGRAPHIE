#include "draw_commands.h"
#include <cstdio>

DrawArraysCommand::DrawArraysCommand(VertexArrayObject& vao, GLsizei count) : m_vao(vao), m_count(count)
{
    // TODO
}

void DrawArraysCommand::draw()
{
    // TODO (ndc 2 p.32)
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, m_count);
}

void DrawArraysCommand::setCount(GLsizei count)
{
    // TODO
    m_count = count;
}

DrawElementsCommand::DrawElementsCommand(VertexArrayObject& vao, GLsizei count, GLenum type) 
: m_vao(vao), m_count(count), m_type(type)
{
    // TODOx
}

void DrawElementsCommand::draw()
{
    // // TODO (ndc 2 p.33)
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_count, m_type, nullptr);
}

void DrawElementsCommand::setCount(GLsizei count)
{
    // TODO
    m_count = count;
}


