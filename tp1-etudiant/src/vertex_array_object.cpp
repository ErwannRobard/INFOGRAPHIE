#include "vertex_array_object.h"
#include <iostream>
VertexArrayObject::VertexArrayObject()
{
    // TODO
    glGenVertexArrays(1, &m_id);
    if(!m_id){
        std::cerr << "Error: failed to create Vertex Array Object" << std::endl;
    }
}

VertexArrayObject::~VertexArrayObject()
{
    // TODO
    if(m_id != 0) {
        glDeleteVertexArrays(1, &m_id);
    }
}

void VertexArrayObject::bind()
{
    // TODO
    glBindVertexArray(m_id);
}

void VertexArrayObject::unbind()
{
    // TODO
    glBindVertexArray(0);
}

void VertexArrayObject::specifyAttribute(BufferObject& buffer, GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // TODO
    bind(); //bind vao
    buffer.bind(); //bind VBO
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(offset * sizeof(float)));
}

