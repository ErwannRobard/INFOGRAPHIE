#include "shader_program.h"

#include <iostream>
#include "shader_object.h"

ShaderProgram::ShaderProgram() : m_id(glCreateProgram())
{
    // TODO
}
    
ShaderProgram::~ShaderProgram()
{        
    // TODO
    glDeleteProgram(m_id);
}
    
GLuint ShaderProgram::getId(){
    return m_id;
}

void ShaderProgram::use()
{
    // TODO
    glUseProgram(m_id);
}
    
void ShaderProgram::attachShaderObject(ShaderObject& s)
{        
    // TODO
    glAttachShader(m_id, s.id());
}
    
void ShaderProgram::link()
{
    // TODO
    glLinkProgram(m_id);

    checkLinkingError();
}

GLint ShaderProgram::getUniformLoc(const char* name)
{
    // TODO
    return glGetUniformLocation(m_id, name);
}
    
void ShaderProgram::checkLinkingError()
{
    GLint success;
    GLchar infoLog[1024];

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 1024, NULL, infoLog);
        glDeleteProgram(m_id);
        std::cout << "Program linking error: " << infoLog << std::endl;
    }
}
