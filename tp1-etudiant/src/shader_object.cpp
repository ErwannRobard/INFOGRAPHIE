#include "shader_object.h"

#include <iostream>

ShaderObject::ShaderObject(GLenum type, const char* code)
{
    // TODO
}
    
ShaderObject::~ShaderObject()
{
    // TODO
}

GLuint ShaderObject::id()
{
    // TODO
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

