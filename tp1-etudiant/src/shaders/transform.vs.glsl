#version 330 core

// TODO

layout(location = 0) in vec4 aPosition;

layout(location = 1) in vec3 color;

uniform mat4 mvp;

out vec3 laCouleurVertex;

void main() {
    gl_Position = mvp * aPosition;

    laCouleurVertex = color;
}