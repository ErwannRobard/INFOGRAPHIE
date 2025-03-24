#version 330 core

// Doit avoir le nom "mvp", sinon modifier resources.cpp
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 mvp;
out vec2 fragUV;

// TODO
void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    fragUV = texcoord;
}

