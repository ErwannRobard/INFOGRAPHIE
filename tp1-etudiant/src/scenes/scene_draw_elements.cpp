#include "scene_draw_elements.h"
#include "utils.h"
#include "vertices_data.h"

SceneDrawElements::SceneDrawElements(Resources& res)
: Scene(res)
, m_coloredSquareReduceDraw(m_coloredSquareReduceVao, 6, GL_UNSIGNED_BYTE)
{
    // TODO bind vao
    m_coloredSquareReduceVao.bind();
    CHECK_GL_ERROR;
    
    //1 vertex with 5 floats, with (x,y) position and (r,g,b) color
    //(r,g,b) with offset 2*sizeof(GLfloat) from position
    //total stride 5*sizeof(GLfloat)
    m_coloredSquareReduceVao.specifyAttribute(m_resources.coloredSquareReduceBuffer, 0, 2, 5, 0);

    
    m_coloredSquareReduceVao.specifyAttribute(m_resources.coloredSquareReduceBuffer, 1, 3, 5, 2);
   
    //bind EBO
    m_resources.coloredSquareReduceIndicesBuffer.bind();

    m_coloredSquareReduceVao.unbind();
}

void SceneDrawElements::run(Window& w)
{
    // TODO call color shader  
    m_resources.color.use();
    m_coloredSquareReduceDraw.draw();
}

// #include "scene_draw_elements.h"
// #include "utils.h"
// #include "vertices_data.h"
// #include <cstdio>      // For printf
// #include <GL/glew.h>   // For OpenGL functions


// void checkGLError(const char* file, int line, const char* msg) {
//     GLenum err;
//     while ((err = glGetError()) != GL_NO_ERROR) {
//         printf("OpenGL error 0x%x at %s:%d: %s\n", err, file, line, msg);
//     }
// }

// #define CHECK_ERROR(msg) checkGLError(__FILE__, __LINE__, msg)


// SceneDrawElements::SceneDrawElements(Resources& res)
//     : Scene(res)
//     , m_coloredSquareReduceVao()
//     , m_coloredSquareReduceDraw(m_coloredSquareReduceVao, 6, GL_UNSIGNED_BYTE)
// {
//     m_coloredSquareReduceVao.bind();
//     CHECK_ERROR("After binding VAO");

//     m_coloredSquareReduceVao.specifyAttribute(m_resources.coloredSquareReduceBuffer, 0, 2, 5, 0);
//     CHECK_ERROR("After specifying attribute 0");

//     m_coloredSquareReduceVao.specifyAttribute(m_resources.coloredSquareReduceBuffer, 1, 3, 5, 2);
//     CHECK_ERROR("After specifying attribute 1");

//     m_resources.coloredSquareReduceIndicesBuffer.bind();
//     CHECK_ERROR("After binding EBO");

//     m_coloredSquareReduceVao.unbind();
//     CHECK_ERROR("After unbinding VAO");
// }

// void SceneDrawElements::run(Window& w)
// {
//     m_resources.color.use();
//     CHECK_ERROR("After using shader");

//     m_coloredSquareReduceDraw.draw();
//     CHECK_ERROR("After glDrawElements");
// }



