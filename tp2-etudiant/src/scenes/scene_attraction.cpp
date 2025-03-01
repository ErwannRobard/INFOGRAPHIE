#include "scene_attraction.h"

#include <iostream>
#include "imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"

// TODO - coordonnées de texture
const GLfloat groundData[] = {
    -45.f, 0.0f, -45.f,  0.0f, 0.0f,
     45.f, 0.0f, -45.f,  6.0f, 0.0f,
    -45.f, 0.0f,  45.f,  0.0f, 6.0f,
     45.f, 0.0f,  45.f,  6.0f, 6.0f,
};

const GLubyte indexes[] = {
    2, 3, 0,
    3, 1, 0
};


SceneAttraction::SceneAttraction(Resources& res, bool& isMouseMotionEnabled)
: Scene(res)
, m_isMouseMotionEnabled(isMouseMotionEnabled)
, m_cameraMode(0)
, m_isOrtho(false)
, m_cameraPosition(0.0f, 5.0f, 0.0f)
, m_cameraOrientation(0.0f, 0.0f)

, m_groundBuffer(GL_ARRAY_BUFFER, sizeof(groundData), groundData, GL_STATIC_DRAW)
, m_groundIndicesBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW)
, m_groundVao()
, m_groundDraw(m_groundVao, 6)

, m_largePlatformAngle(0.0f)
, m_smallPlatformAngle{0.0f}
, m_cupsAngles{0.0f}

, m_suzanne("../models/suzanne.obj")
, m_cup("../models/cup.obj")
, m_cupPlate("../models/plate.obj")
, m_smallPlatform("../models/smallPlatform.obj")
, m_largePlatform("../models/largePlatform.obj")
, m_cube("../models/cube.obj")

, m_groundTexture("../textures/grassSeamless.jpg")
, m_suzanneTexture("../textures/suzanneTextureShade.png")
, m_cupTextureAtlas("../textures/cupTextureAtlas.png")
, m_smallPlatformTexture("../textures/smallPlatform.png")
, m_largePlatformTexture("../textures/largePlatform.png")
{
    // TODO - spécifier les attributs
    
    m_groundVao.bind();
    m_groundIndicesBuffer.bind();
    m_groundBuffer.bind();

    m_groundVao.specifyAttribute(m_groundBuffer, 0, 3, 5, 0);

    m_groundVao.specifyAttribute(m_groundBuffer, 1, 2, 5, 3);

    m_groundVao.unbind();

    // TODO - init des textures

    m_groundTexture.setWrap(GL_REPEAT);
    m_groundTexture.setFiltering(GL_LINEAR);
    m_groundTexture.enableMipmap();

    m_suzanneTexture.setWrap(GL_CLAMP_TO_EDGE);
    m_suzanneTexture.setFiltering(GL_LINEAR);
    m_suzanneTexture.enableMipmap();

    m_cupTextureAtlas.setWrap(GL_CLAMP_TO_EDGE);
    m_cupTextureAtlas.setFiltering(GL_LINEAR);
    m_cupTextureAtlas.enableMipmap();

    m_smallPlatformTexture.setWrap(GL_CLAMP_TO_EDGE);
    m_smallPlatformTexture.setFiltering(GL_LINEAR);
    m_smallPlatformTexture.enableMipmap();

    m_largePlatformTexture.setWrap(GL_CLAMP_TO_EDGE);
    m_largePlatformTexture.setFiltering(GL_NEAREST);
    m_largePlatformTexture.enableMipmap();
}

SceneAttraction::~SceneAttraction()
{
}

void SceneAttraction::run(Window& w, double dt)
{    
    ImGui::Begin("Scene Parameters");
    ImGui::Combo("Camera mode", &m_cameraMode, CAMERA_MODE_NAMES, N_CAMERA_MODE_NAMES);
    ImGui::Checkbox("Orthographic camera?", &m_isOrtho);
    ImGui::End();
    
    updateInput(w, dt);    
    
    m_largePlatformAngle += 0.5 * dt;
    for (int i = 0; i < 3; i++)
    {
        m_smallPlatformAngle[i] += 0.5 * dt;
        for (int j = 0; j < 4; j++)
            m_cupsAngles[i][j] += (0.5 + j * 0.5f) * dt;
    }

    glm::mat4 model, proj, view, mvp;
    
    proj = getProjectionMatrix(w);
    
    if (m_cameraMode == 0 || m_cameraMode == 2)
        view = getCameraFirstPerson();
    else
        view = getCameraThirdPerson();
    
    // TODO - dessin de la scène

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
    mvp = proj * view * model;

    m_resources.texture.use();
    m_groundTexture.use();
    glUniformMatrix4fv(m_resources.mvpLocationTexture, 1, GL_FALSE, &mvp[0][0]);
    m_groundDraw.draw();

    glm::vec3 cubePositions[4] = {
        glm::vec3( 30.f, 3.f,  0.f),
        glm::vec3(-30.f, 3.f,  0.f),
        glm::vec3(  0.f, 3.f, 30.f),
        glm::vec3(  0.f, 3.f,-30.f)
    };

    glm::vec3 cubeColors[4] = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    };

    m_resources.colorUniform.use();
    for (int i = 0; i < 4; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model = glm::scale(model, glm::vec3(6.0f));
        mvp = proj * view * model;

        glUniformMatrix4fv(m_resources.mvpLocationColorUniform, 1, GL_FALSE, &mvp[0][0]);
        glUniform3fv(m_resources.colorLocationColorUniform, 1, &cubeColors[i][0]);
        m_cube.draw();
    }

    model = glm::mat4(1.0f);
    model = glm::rotate(model, m_largePlatformAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = proj * view * model;

    m_resources.texture.use();
    glUniformMatrix4fv(m_resources.mvpLocationTexture, 1, GL_FALSE, &mvp[0][0]);
    m_largePlatformTexture.use();
    m_largePlatform.draw();

    
    for (int i = 0; i < 3; i++)
    {
        float angle = i * glm::radians(120.0f);

        glm::mat4 groupModel = glm::mat4(1.0f);

        groupModel = glm::rotate(groupModel, m_largePlatformAngle, glm::vec3(0,1,0));  
        groupModel = glm::rotate(groupModel, angle, glm::vec3(0,1,0));  
        groupModel = glm::translate(groupModel, glm::vec3(15.0f, 0.0f, 0.0f));  
        groupModel = glm::rotate(groupModel, m_smallPlatformAngle[i], glm::vec3(0,1,0)); 

        mvp = proj * view * groupModel;
        m_resources.texture.use();
        glUniformMatrix4fv(m_resources.mvpLocationTexture, 1, GL_FALSE, &mvp[0][0]);
        m_smallPlatformTexture.use();
        m_smallPlatform.draw();

        for (int j = 0; j < 4; j++)
        {
            float cupAngle = j * glm::radians(90.0f);  

            glm::mat4 cupBase = groupModel;
           
            cupBase = glm::rotate(cupBase, cupAngle, glm::vec3(0,1,0));  
            cupBase = glm::translate(cupBase, glm::vec3(6.0f, 0.0f, 0.0f));  
            cupBase = glm::rotate(cupBase, m_cupsAngles[i][j], glm::vec3(0,1,0));  

            glm::mat4 plateModel = cupBase;
            mvp = proj * view * plateModel;
            m_resources.cup.use();
            glUniformMatrix4fv(m_resources.mvpLocationCup, 1, GL_FALSE, &mvp[0][0]);
            m_cupTextureAtlas.use();
            m_cupPlate.draw();

            glm::mat4 finalCupModel = cupBase;
            finalCupModel = glm::translate(finalCupModel, glm::vec3(0.0f, 0.12f, 0.0f)); // Slight vertical offset
            mvp = proj * view * finalCupModel;
            glUniformMatrix4fv(m_resources.mvpLocationCup, 1, GL_FALSE, &mvp[0][0]);
            glUniform1i(m_resources.textureIndexLocationCup, j);
            glUniform1i(m_resources.isPlateLocationCup, 0);
            m_cupTextureAtlas.use();
            m_cup.draw();

            if (i == 0 && j == 0)
            {
                if(m_cameraMode != 2){

                glm::mat4 monkeyModel = finalCupModel;
                monkeyModel = glm::scale(monkeyModel, glm::vec3(2.0f));
                monkeyModel = glm::translate(monkeyModel, glm::vec3(0.0f, 0.5f, 0.0f));
                mvp = proj * view * monkeyModel;
                m_resources.texture.use();
                glUniformMatrix4fv(m_resources.mvpLocationTexture, 1, GL_FALSE, &mvp[0][0]);
                m_suzanneTexture.use();
                m_suzanne.draw();
                } 
                else    //no draw on monke cameraMode
                {
                    glm::mat4 suzanneMyLove = finalCupModel;
                    suzanneMyLove = glm::scale(suzanneMyLove, glm::vec3(2.0f));
                    m_cameraPosition = glm::vec3(suzanneMyLove[3]);
                    m_cameraPosition.y = 3.8f;
                    m_cameraOrientation.y = std::atan2(suzanneMyLove[2].x, suzanneMyLove[0].x);
                }
            }
        }
    }
}

void SceneAttraction::updateInput(Window& w, double dt)
{
    // Mouse input
    int x = 0, y = 0;
    if (m_isMouseMotionEnabled)
        w.getMouseMotion(x, y);
    const float MOUSE_SENSITIVITY = 0.1;
    float cameraMouvementX = y * MOUSE_SENSITIVITY;
    float cameraMouvementY = x * MOUSE_SENSITIVITY;
    
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

glm::mat4 SceneAttraction::getCameraFirstPerson()
{
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), -m_cameraOrientation.x, glm::vec3(1.0f, 0.0f, 0.0f)); 
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), -m_cameraOrientation.y, glm::vec3(0.0f, 1.0f, 0.0f)); 
    glm::mat4 rotation = rotationY * rotationX;

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_cameraPosition); 
    glm::mat4 view = rotation * translation;
    
    return view;
}

glm::mat4 SceneAttraction::getCameraThirdPerson()
{
    float radius = 36.0f; 
    float theta = m_cameraOrientation.x;
    float phi = m_cameraOrientation.y + glm::half_pi<float>();

    glm::vec3 cameraPos = m_cameraPosition + glm::vec3(
        radius * glm::cos(theta) * glm::sin(phi),
        radius * glm::sin(phi),                
        radius * glm::cos(theta) * glm::sin(phi)
    );

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    return glm::lookAt(cameraPos, m_cameraPosition, up);
}


glm::mat4 SceneAttraction::getProjectionMatrix(Window& w)
{
    // TODO
    const float SCREEN_SIZE_ORTHO = 10.0f;
    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 300.0f;
    const float FOV = glm::radians(70.0f);

    glm::mat4 proj;

    if (m_isOrtho)
    {
        proj = glm::ortho(-SCREEN_SIZE_ORTHO / 2.0f, SCREEN_SIZE_ORTHO / 2.0f, 
                          -SCREEN_SIZE_ORTHO / 2.0f, SCREEN_SIZE_ORTHO / 2.0f, 
                          NEAR_PLANE, FAR_PLANE);
    }
    else
    {
        float aspectRatio = static_cast<float>(w.getWidth()) / static_cast<float>(w.getHeight());
        proj = glm::perspective(FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);
    }

    return proj;
}

