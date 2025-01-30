#include "buffer_object.h"

BufferObject::BufferObject(): m_id(0), m_type(GL_ARRAY_BUFFER)
{
    // TODO
    glGenBuffers(1, &m_id);
}

BufferObject::BufferObject(GLenum type, GLsizeiptr dataSize, const void* data, GLenum usage)
: BufferObject()
{
    // TODO
    allocate(type, dataSize, data, usage);
}

BufferObject::~BufferObject()
{
    // TODO
    if(m_id != 0){
        glDeleteBuffers(1, &m_id);
    }
}

void BufferObject::bind()
{
    // TODO
    glBindBuffer(m_type, m_id);
}

void BufferObject::allocate(GLenum type, GLsizeiptr dataSize, const void* data, GLenum usage)
{
    // TODO
    //bind the buffer, specify type and upload data to gpu
    m_type = type;
    bind();
    glBufferData(type, dataSize, data, usage);
}

void BufferObject::update(GLsizeiptr dataSize, const void* data)
{
    // TODO
    bind();
    glBufferSubData(m_type, 0, dataSize, data);
}

void* BufferObject::mapBuffer()
{
    // TODO
    bind();
    return glMapBuffer(m_type,  GL_READ_WRITE); //map the buf to cpu-accessibe memory
}

void BufferObject::unmapBuffer()
{
    // TODO
    bind();
    glUnmapBuffer(m_type);
}



// #include "buffer_object.h"

// BufferObject::BufferObject()
// {
//     // TODO (ndc 2 p.15)
//     glGenBuffers(1, &m_id);
// }

// BufferObject::BufferObject(GLenum type)
//     : m_type(type)
// {
//   glGenBuffers(1, &m_id);
// }

// BufferObject::BufferObject(GLenum type, GLsizeiptr dataSize, const void* data, GLenum usage)
// : BufferObject(type)
// {
//     // TODO
//     allocate(type, dataSize, data, usage); 
// }

// BufferObject::~BufferObject()
// {
//     // TODO
//     glDeleteBuffers(1, &m_id);
// }

// void BufferObject::bind()
// {
//     // TODO
//     glBindBuffer(m_type, m_id);
// }

// void BufferObject::allocate(GLenum type, GLsizeiptr dataSize, const void* data, GLenum usage)
// {
//     // TODO
//     m_type = type;
//     bind(); //permet de verifier quel buffer on manipule
//     glBufferData(type, dataSize, data, usage);
// }

// void BufferObject::update(GLsizeiptr dataSize, const void* data)
// {
//     // TODO
//     bind();
//     glBufferSubData(m_type, 0, dataSize, data);
// }

// void* BufferObject::mapBuffer()
// {
//     // TODO
//     bind();
//     return glMapBuffer(m_type, GL_WRITE_ONLY);
// }

// void BufferObject::unmapBuffer()
// {
//     // TODO
//     bind();
//     glUnmapBuffer(m_type);
// }

