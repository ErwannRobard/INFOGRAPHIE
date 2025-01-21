#include "shader_program.h"

#include <iostream>
#include "shader_object.h"

ShaderProgram::ShaderProgram()
{
    // TODO
}
    
ShaderProgram::~ShaderProgram()
{        
    // TODO
}
    
void ShaderProgram::use()
{
    // TODO
}
    
void ShaderProgram::attachShaderObject(ShaderObject& s)
{        
    // TODO
}
    
void ShaderProgram::link()
{
    // TODO
}

GLint ShaderProgram::getUniformLoc(const char* name)
{
    // TODO
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
