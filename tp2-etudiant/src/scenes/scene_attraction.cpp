#include "scene_attraction.h"

#include <iostream>
#include "imgui/imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"

// TODO - coordonnées de texture
const GLfloat groundData[] = {
    -45.f, 0.0f, -45.f,  0.0f, 0.0f,
     45.f, 0.0f, -45.f,  0.0f, 0.0f,
    -45.f, 0.0f,  45.f,  0.0f, 0.0f,
     45.f, 0.0f,  45.f,  0.0f, 0.0f,
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

    // m_groundVao.specifyAttribute(m_groundBuffer, m_groundIndicesBuffer);
    m_groundVao.specifyAttribute(m_groundBuffer, 0, 3, 5, 0);

    m_groundVao.specifyAttribute(m_groundBuffer, 1, 2, 5, 3);

    m_groundVao.unbind();

    // TODO - init des textures
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
    //glUniform
    glUseProgram(1);
    m_groundDraw.draw();

    // glm::mat4 groundModel = glm::mat4(1.0f);
    // m_groundTexture.bind();
    // mvp = proj * view * groundModel;
    // setUniformMatrix("u_MVP", mvp);
    // m_groundDraw.draw();

    // Debut de code pour le dessin des groupes de tasses (et obtenir la position du singe)
    glm::vec3 monkeyPos = glm::vec3(0.0f);
    float monkeyHeading = 0.0f;
    for (int i = 0; i < 3; i++)
    {
        
        for (int j = 0; j < 4; j++)
        {
            // Mettez la matrice model finale de la tasse dans cette matrice
            // pour être en mesure d'avoir la position et orientation du singe
            glm::mat4 cupModelMat = glm::mat4(1.0f);
            if (i == 0 && j == 0)
            {
                monkeyPos = glm::vec3(cupModelMat[3].x, 1.3, cupModelMat[3].z);
                monkeyHeading = atan2(cupModelMat[2].x, cupModelMat[0].x);
            }
        }
    }
    
    // Laissez ce code à la fin de la méthode
    if (m_cameraMode == 2)
    {
        m_cameraPosition = monkeyPos;
        m_cameraPosition.y = 3.8;
        m_cameraOrientation.y = monkeyHeading;
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
    glm::mat4 view = glm::mat4(1.0f);
    
    view = glm::rotate(view, -m_cameraOrientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, -m_cameraOrientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    
    view = glm::translate(view, -m_cameraPosition);
    
    return view;
}

glm::mat4 SceneAttraction::getCameraThirdPerson()
{
    const float radius = 36.0f;
    
    float camX = radius * cos(m_cameraOrientation.x) * sin(m_cameraOrientation.y);
    float camY = radius * sin(m_cameraOrientation.x);
    float camZ = radius * cos(m_cameraOrientation.x) * cos(m_cameraOrientation.y);
    
    glm::vec3 cameraPos = glm::vec3(camX, camY, camZ);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    return glm::lookAt(cameraPos, target, up);
}


glm::mat4 SceneAttraction::getProjectionMatrix(Window& w)
{
    // TODO
    const float SCREEN_SIZE_ORTHO = 10.0f;
    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 300.0f;
    const float FOV = glm::radians(70.0f);

    glm::mat4 proj;

    if (m_isOrtho){
        proj = glm::ortho(-SCREEN_SIZE_ORTHO / 2.0f, SCREEN_SIZE_ORTHO / 2.0f, 
                          -SCREEN_SIZE_ORTHO / 2.0f, SCREEN_SIZE_ORTHO / 2.0f, 
                          NEAR_PLANE, FAR_PLANE);    
    } else {
        float aspectRatio = static_cast<float>(w.getWidth()) / static_cast<float>(w.getHeight());
        proj = glm::perspective(FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);
    }
    return proj;
}

