#version 330 core

// TODO

layout(location = 0) in vec4 aPosition;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * aPosition;
}