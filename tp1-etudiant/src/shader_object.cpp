#include "shader_object.h"

#include <iostream>

ShaderObject::ShaderObject(GLenum type, const char* code) : m_id(glCreateShader(type))
{
    // TODO (ndc 2 page 63)
    glShaderSource(m_id, 1, &code, NULL);
    glCompileShader(m_id);

    checkCompilingError();
}
    
ShaderObject::~ShaderObject()
{
    // TODO (suppression du shader)
    glDeleteShader(m_id);
}

GLuint ShaderObject::id()
{
    // TODO
    return m_id;
}

void ShaderObject::checkCompilingError()
{
    GLint success;
    GLchar infoLog[1024];

    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_id, 1024, NULL, infoLog);
        glDeleteShader(m_id);
        std::cout << "Shader compile error: " << infoLog << std::endl;
    }
}

