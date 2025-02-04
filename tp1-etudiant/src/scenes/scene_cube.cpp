#include "scene_cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertices_data.h"

SceneCube::SceneCube(Resources& res)
: Scene(res), m_cubeDraw(m_cubeVao, 36)
{
    // TODO
    m_cubeVao.bind();

    m_cubeBuffer.allocate(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    m_cubeIndicesBuffer.allocate(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexes), cubeIndexes, GL_STATIC_DRAW);

    m_cubeVao.specifyAttribute(m_cubeBuffer, 0, 3, 6 * sizeof(float), 0);

    m_cubeVao.specifyAttribute(m_cubeBuffer, 1, 3, 6 * sizeof(float), 3 * sizeof(float));
}

void SceneCube::run(Window& w)
{
    m_rotationAngleDegree += 0.5f;
	    // TODO
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(m_rotationAngleDegree), glm::vec3(0.1f, 1.0f, 0.1f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.0f));

    float aspectRatio = static_cast<float>(w.getWidth()) / static_cast<float>(w.getHeight());
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.1f, 10.0f);

    glm::mat4 mvp = projection * view * model;

    m_resources.mvpLocationTransformColorAttrib = m_resources.transformColorAttrib.getUniformLoc("mvp");

    m_resources.transformColorAttrib.use();

    glUniformMatrix4fv(m_resources.mvpLocationTransformColorAttrib, 1, GL_FALSE, glm::value_ptr(mvp));

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_cubeDraw.draw();
}

