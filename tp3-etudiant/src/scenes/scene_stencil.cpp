#include "scene_stencil.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

#include "utils.h"

#include <iostream>


SceneStencil::SceneStencil(Resources& res, bool& isMouseMotionEnabled)
: Scene(res)
, m_isMouseMotionEnabled(isMouseMotionEnabled)
, m_cameraPosition(0, 1, 0)
, m_cameraOrientation(0)

, m_groundBuffer(GL_ARRAY_BUFFER, 5 * 4 * sizeof(GLfloat), groundData, GL_STATIC_DRAW)
, m_groundIndicesBuffer(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLubyte), indexes, GL_STATIC_DRAW)
, m_groundVao()
, m_groundDraw(m_groundVao, 6)

, m_suzanne("../models/suzanne.obj")
, m_rock("../models/rock.obj")
, m_glass("../models/glass.obj")

, m_groundTexture("../textures/grassSeamless.jpg")
, m_suzanneTexture("../textures/suzanneTextureShade.png")
, m_suzanneWhiteTexture("../textures/suzanneWhite.png")
, m_rockTexture("../textures/rockTexture.png")
, m_glassTexture("../textures/glass.png")
, m_whiteGridTexture("../textures/whiteGrid.png")
{
    m_groundVao.specifyAttribute(m_groundBuffer, 0, 3, 5, 0);
    m_groundVao.specifyAttribute(m_groundBuffer, 1, 2, 5, 3);
    
    m_groundVao.bind();
    m_groundIndicesBuffer.bind();
    m_groundVao.unbind();

    m_groundTexture.setFiltering(GL_LINEAR);
    m_groundTexture.setWrap(GL_REPEAT);
    m_groundTexture.enableMipmap();
    
    m_suzanneTexture.setFiltering(GL_LINEAR);
    m_suzanneTexture.setWrap(GL_CLAMP_TO_EDGE);
    
    m_suzanneWhiteTexture.setFiltering(GL_LINEAR);
    m_suzanneWhiteTexture.setWrap(GL_CLAMP_TO_EDGE);

    m_rockTexture.setFiltering(GL_LINEAR);
    m_rockTexture.setWrap(GL_CLAMP_TO_EDGE);
    
    m_glassTexture.setFiltering(GL_LINEAR);
    m_glassTexture.setWrap(GL_CLAMP_TO_EDGE);
    
    m_whiteGridTexture.setFiltering(GL_LINEAR);
    m_whiteGridTexture.setWrap(GL_REPEAT);
}

SceneStencil::~SceneStencil(){}

void SceneStencil::run(Window& w, double dt)
{
    updateInput(w, dt);
    
    // TODO Dessin de la scène de stencil
    // utiliser les shaders texture et simpleColor ici

    glm::mat4 proj = getProjectionMatrix(w);    
    glm::mat4 view = getCameraFirstPerson();    
    glm::mat4 projView = proj * view;

    //all matrix
    glm::mat4 suzanneModel = glm::translate(glm::mat4(1.0f), glm::vec3(-14, -0.1f, 2));
    glm::mat4 rockModel = glm::translate(glm::mat4(1.0f), glm::vec3(-10, 0.4f, 0));
    rockModel = glm::scale(rockModel, glm::vec3(2.0f));
    glm::mat4 groundModel = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.1f, 0));

    glm::mat4 suzanneMVP = projView * suzanneModel;
    glm::mat4 rockMVP = projView * rockModel;
    glm::mat4 groundMVP = projView * groundModel;

    //start stencil test
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilMask(0xFF);

    //stencil replace to 1 where monkey is
    glStencilFunc(GL_ALWAYS, 1, 0xFF);       
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    ShaderProgram& texturedShader = m_resources.texture;
    texturedShader.use();
    glUniformMatrix4fv(m_resources.mvpLocationTexture, 1, GL_FALSE, glm::value_ptr(suzanneMVP));
    m_suzanneTexture.use(0);
    glUniform1i(texturedShader.getUniformLoc("colorTexture"), 0);
    m_suzanne.draw();

    //draw rock without modifying stencil buffer
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0x00);

    glStencilFunc(GL_ALWAYS, 0, 0xFF);          //always pass stencil as we want to always see monkey

    glUniformMatrix4fv(m_resources.mvpLocationTexture, 1, GL_FALSE, glm::value_ptr(rockMVP));
    m_rockTexture.use(0);
    glUniform1i(texturedShader.getUniformLoc("colorTexture"), 0);
    m_rock.draw();

    //where stencil == 1 && depth test fails, Silhouette pass using simpleCOlor
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glDepthFunc(GL_GREATER);                    //where monke is BEHIND something
    glDepthMask(GL_FALSE);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);                //prevent z-fighting since model and silouette same position

    ShaderProgram& simple = m_resources.simpleColor;
    simple.use();
    glUniformMatrix4fv(m_resources.mvpLocationSimpleColor, 1, GL_FALSE, glm::value_ptr(suzanneMVP));
    m_suzanneWhiteTexture.use(0);
    glUniform1i(simple.getUniformLoc("colorTexture"), 0);
    m_suzanne.draw();

    glDisable(GL_POLYGON_OFFSET_FILL);
    glDepthMask(GL_TRUE);

    //textured suzanne when visible
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glDepthFunc(GL_LESS);
    glStencilMask(0x00);

    texturedShader.use();
    glUniformMatrix4fv(m_resources.mvpLocationTexture, 1, GL_FALSE, glm::value_ptr(suzanneMVP));
    m_suzanneTexture.use(0);
    glUniform1i(texturedShader.getUniformLoc("colorTexture"), 0);
    m_suzanne.draw();

    //draw ground no stencil
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0xFF);

    glUniformMatrix4fv(m_resources.mvpLocationTexture, 1, GL_FALSE, glm::value_ptr(groundMVP));
    m_groundTexture.use(0);
    glUniform1i(texturedShader.getUniformLoc("colorTexture"), 0);
    m_groundVao.bind();
    m_groundDraw.draw();
    m_groundVao.unbind();
}

void SceneStencil::updateInput(Window& w, double dt)
{
    // Mouse input
    int x = 0, y = 0;
    if (m_isMouseMotionEnabled)
        w.getMouseMotion(x, y);
    const float MOUSE_SENSITIVITY = 0.1;
    float cameraMouvementX = y * MOUSE_SENSITIVITY;
    float cameraMouvementY = x * MOUSE_SENSITIVITY;;
    
    const float KEYBOARD_MOUSE_SENSITIVITY = 1.5f;
    if (w.getKeyHold(Window::Key::UP))
        cameraMouvementX -= KEYBOARD_MOUSE_SENSITIVITY;
    if (w.getKeyHold(Window::Key::DOWN))
        cameraMouvementX += KEYBOARD_MOUSE_SENSITIVITY;
    if (w.getKeyHold(Window::Key::LEFT))
        cameraMouvementY -= KEYBOARD_MOUSE_SENSITIVITY;
    if (w.getKeyHold(Window::Key::RIGHT))
        cameraMouvementY += KEYBOARD_MOUSE_SENSITIVITY;
    
    m_cameraOrientation.y -= cameraMouvementY * dt;
    m_cameraOrientation.x -= cameraMouvementX * dt;

    // Keyboard input
    glm::vec3 positionOffset = glm::vec3(0.0);
    const float SPEED = 10.f;
    if (w.getKeyHold(Window::Key::W))
        positionOffset.z -= SPEED;
    if (w.getKeyHold(Window::Key::S))
        positionOffset.z += SPEED;
    if (w.getKeyHold(Window::Key::A))
        positionOffset.x -= SPEED;
    if (w.getKeyHold(Window::Key::D))
        positionOffset.x += SPEED;
        
    if (w.getKeyHold(Window::Key::Q))
        positionOffset.y -= SPEED;
    if (w.getKeyHold(Window::Key::E))
        positionOffset.y += SPEED;

    positionOffset = glm::rotate(glm::mat4(1.0f), m_cameraOrientation.y, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(positionOffset, 1);
    m_cameraPosition += positionOffset * glm::vec3(dt);
}

