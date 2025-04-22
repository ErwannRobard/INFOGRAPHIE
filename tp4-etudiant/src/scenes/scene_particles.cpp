#include "scene_particles.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

#include "utils.h"
#include "shader_object.h"

#include <iostream>


static const unsigned int MAX_N_PARTICULES = 1000;
static Particle particles[MAX_N_PARTICULES] = { {{0,0,0},{0,0,0},{0,0,0,0}, {0,0},0} };

SceneParticles::SceneParticles(bool& isMouseMotionEnabled)
: Scene()
, m_isMouseMotionEnabled(isMouseMotionEnabled)
, m_cameraOrientation(0)
, m_totalTime(0.0f)
, m_cumulativeTime(0.0f)
, m_tfo(0)
, m_vao(0)
, m_vbo{0, 0}
, m_nParticles(1)
, m_nMaxParticles(MAX_N_PARTICULES)
, m_transformFeedbackShaderProgram("TransformFeedback")
, m_timeLocationTransformFeedback(-1)
, m_dtLocationTransformFeedback(-1)
, m_particleShaderProgram("ParticleShader")
, m_modelViewLocationParticle(-1)
, m_projectionLocationParticle(-1)
, m_flameTexture("../textures/flame.png")
, m_menuVisible(true)
{
    initializeShader();
    initializeTexture();

    glEnable(GL_PROGRAM_POINT_SIZE);
    CHECK_GL_ERROR;
    // TODO

    glGenBuffers(2, m_vbo);
    CHECK_GL_ERROR;
    glGenVertexArrays(1, &m_vao);
    CHECK_GL_ERROR;
    glGenTransformFeedbacks(1, &m_tfo);
    CHECK_GL_ERROR;

    //init VBO VAO
    GLsizeiptr particleDataSize = m_nMaxParticles * sizeof(Particle);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, particleDataSize, NULL, GL_STREAM_DRAW);
    CHECK_GL_ERROR;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, particleDataSize, NULL, GL_STREAM_DRAW);
    CHECK_GL_ERROR;

    glBindVertexArray(m_vao);
    CHECK_GL_ERROR;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    CHECK_GL_ERROR;

    //1 stride = 1 particle struct of 52 bytes
    GLsizei stride = sizeof(Particle);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);  //position
    glEnableVertexAttribArray(0);
    CHECK_GL_ERROR;

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)12); //veclocity, vec3 12 bytes from position
    glEnableVertexAttribArray(1);
    CHECK_GL_ERROR;

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)24); //color, vec3 12 bytes from velocity
    glEnableVertexAttribArray(2);
    CHECK_GL_ERROR;

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)40); //size, vec4 16 byte from color
    glEnableVertexAttribArray(3);
    CHECK_GL_ERROR;

    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, (void*)48); //timeToLive, vec2 8 byte from size
    glEnableVertexAttribArray(4);
    CHECK_GL_ERROR;

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR;

    //TFO
    glGenTransformFeedbacks(1, &m_tfo);
    CHECK_GL_ERROR;
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfo);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    CHECK_GL_ERROR;
}

SceneParticles::~SceneParticles()
{
    // TODO
    glDeleteBuffers(2, m_vbo);
    CHECK_GL_ERROR;
    glDeleteVertexArrays(1, &m_vao);
    CHECK_GL_ERROR;
    glDeleteTransformFeedbacks(1, &m_tfo);
    CHECK_GL_ERROR;
}

void SceneParticles::run(Window& w, double dt)
{
    updateInput(w, dt);
    drawMenu();

    glm::mat4 view = getCameraThirdPerson(2.5);
    glm::mat4 projPersp = getProjectionMatrix(w);
    glm::mat4 modelView = view;

    m_totalTime += dt;
    m_cumulativeTime += dt;
    if (dt == 0.0f)
        m_nParticles = 1;

    //Double Buffering avec static local variable to track the source buffer index across frames
    static int currentSourceBuffer = 0; //Buffer to READ from
    int destinationBuffer = 1 - currentSourceBuffer; //Buffer to WRITE

    //Transform Feedback
    m_transformFeedbackShaderProgram.use();
    glUniform1f(m_timeLocationTransformFeedback, m_totalTime);
    glUniform1f(m_dtLocationTransformFeedback, (float)dt);

    // TODO: buffer binding
    // TODO: update particles (glDrawArrays)

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[currentSourceBuffer]); //reaad from current source
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfo);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[destinationBuffer]); //write to destination

    glEnable(GL_RASTERIZER_DISCARD);

    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, m_nParticles);
    glEndTransformFeedback();

    glDisable(GL_RASTERIZER_DISCARD);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    //keep VAO bound, but unbind ARRAY_BUFFER, will rebind for drawing
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // TODO: swap buffers 
    //buffer we just wrote to (destinationBuffer) becomes the source for drawing current frame, then source of next frame<s update
    currentSourceBuffer = destinationBuffer;


    m_particleShaderProgram.use();
    m_flameTexture.use(0);

    glUniformMatrix4fv(m_modelViewLocationParticle, 1, GL_FALSE, &modelView[0][0]);
    glUniformMatrix4fv(m_projectionLocationParticle, 1, GL_FALSE, &projPersp[0][0]);

    // TODO: buffer binding
    // TODO: Draw particles without depth write and with blending

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    //Bind 
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[currentSourceBuffer]); //read from the buffer that was just updated

    //draw the updated particles as points
    glDrawArrays(GL_POINTS, 0, m_nParticles);

    //restore render
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    //unbind at end
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //update nmb particule
    if (m_cumulativeTime > 1.0f / 60.0f)
    {
        m_cumulativeTime = 0.0f;
        if (++m_nParticles > m_nMaxParticles)
            m_nParticles = m_nMaxParticles;
    }
}

void SceneParticles::updateInput(Window& w, double dt)
{        
    int x = 0, y = 0;
    if (m_isMouseMotionEnabled)
        w.getMouseMotion(x, y);
    const float MOUSE_SENSITIVITY = 0.1;
    float cameraMouvementX = y * MOUSE_SENSITIVITY;
    float cameraMouvementY = x * MOUSE_SENSITIVITY;
    
    const float KEYBOARD_MOUSE_SENSITIVITY = 1.5f;
    if (w.getKeyHold(Window::Key::UP))
        cameraMouvementX += KEYBOARD_MOUSE_SENSITIVITY;
    if (w.getKeyHold(Window::Key::DOWN))
        cameraMouvementX -= KEYBOARD_MOUSE_SENSITIVITY;
    if (w.getKeyHold(Window::Key::LEFT))
        cameraMouvementY += KEYBOARD_MOUSE_SENSITIVITY;
    if (w.getKeyHold(Window::Key::RIGHT))
        cameraMouvementY -= KEYBOARD_MOUSE_SENSITIVITY;
    
    m_cameraOrientation.y -= cameraMouvementY * dt;
    m_cameraOrientation.x -= cameraMouvementX * dt;
}

void SceneParticles::drawMenu()
{
    if (!m_menuVisible) return;

    ImGui::Begin("Scene Parameters");
    ImGui::End();
}


void SceneParticles::initializeShader()
{
    {
        std::string vertexCode = readFile("shaders/particle.vs.glsl");
        std::string geometryCode = readFile("shaders/particle.gs.glsl");
        std::string fragmentCode = readFile("shaders/particle.fs.glsl");

        ShaderObject vertex("particule.vs.glsl", GL_VERTEX_SHADER, vertexCode.c_str());
        ShaderObject geometry("particule.gs.glsl", GL_GEOMETRY_SHADER, geometryCode.c_str());
        ShaderObject fragment("particule.fs.glsl",GL_FRAGMENT_SHADER, fragmentCode.c_str());
        m_particleShaderProgram.attachShaderObject(vertex);
        m_particleShaderProgram.attachShaderObject(geometry);
        m_particleShaderProgram.attachShaderObject(fragment);
        m_particleShaderProgram.link();

        m_particleShaderProgram.use();
        m_modelViewLocationParticle = m_particleShaderProgram.getUniformLoc("modelView");
        m_projectionLocationParticle = m_particleShaderProgram.getUniformLoc("projection");
    }

    // Transform feedback shader
    {
        std::string vertexCode = readFile("shaders/transformFeedback.vs.glsl");

        ShaderObject vertex("transformFeedback.vs.glsl", GL_VERTEX_SHADER, vertexCode.c_str());
        m_transformFeedbackShaderProgram.attachShaderObject(vertex);

        // TODO
        //utiliser la méthode setTransformFeedbackVaryings avant de faire le link
        const char* varyings[] = {
            "positionMod", "velocityMod", "colorMod", "sizeMod", "timeToLiveMod"
        };
        m_transformFeedbackShaderProgram.setTransformFeedbackVaryings(varyings, 5, GL_INTERLEAVED_ATTRIBS);

        m_transformFeedbackShaderProgram.link();

        //use program to get uniform locations
        m_transformFeedbackShaderProgram.use();
        m_timeLocationTransformFeedback = m_transformFeedbackShaderProgram.getUniformLoc("time");
        m_dtLocationTransformFeedback = m_transformFeedbackShaderProgram.getUniformLoc("dt");
    }
}

void SceneParticles::initializeTexture()
{
    m_flameTexture.setFiltering(GL_LINEAR);
    m_flameTexture.setWrap(GL_CLAMP_TO_EDGE);
}

