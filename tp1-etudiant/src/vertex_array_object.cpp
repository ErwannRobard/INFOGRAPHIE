#include "vertex_array_object.h"

VertexArrayObject::VertexArrayObject()
{
    // TODO (ndc 2 p.21)
    glGenVertexArrays(1, &m_id);
}

VertexArrayObject::~VertexArrayObject()
{
    // TODO (ndc 2  p.21) sert a effacer la memoire sinon ca reste sur la carte graphique
    glDeleteVertexArrays(1, &m_id);
}

void VertexArrayObject::bind()
{
    // TODO (ndc 2  p.21)
    glBindVertexArray(m_id);
}

void VertexArrayObject::unbind()
{
    // TODO (ndc 2  p.23)
    glBindVertexArray(0);
}

void VertexArrayObject::specifyAttribute(BufferObject& buffer, GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // TODO (ndc 2 p.36)
    bind();
    buffer.bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void *)offset);
}
