#ifndef RESOURCES_H
#define RESOURCES_H

#include "shader_program.h"

#include "buffer_object.h"

class Resources
{
public:
    Resources();

    // Shaders
    ShaderProgram basic;
    ShaderProgram color;
    
    ShaderProgram transformColor; 
    GLint mvpLocationColor;

    ShaderProgram transformSolid; 
    GLint mvpLocationSolid;
    GLint locationColorSolid;

    ShaderProgram transformColorAttrib;
    GLint mvpLocationTransformColorAttrib;

    ShaderProgram transformSolidColor;
    GLint mvpLocationTransformSolidColor;
    GLint colorLocationTransformSolidColor;
    
    // Buffer Objects    
    BufferObject coloredTriangleBuffer;
    
    BufferObject coloredSquareReduceBuffer;
    BufferObject coloredSquareReduceIndicesBuffer;

    void initShaderProgram(ShaderProgram& program, const char* vertexSrcPath, const char* fragmentSrcPath);
private:    
    
};

#endif // RESOURCES_H
