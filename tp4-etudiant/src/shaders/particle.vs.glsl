#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 size;
layout (location = 4) in float timeToLive;

uniform mat4 modelView;

out ATTRIB_VS_OUT
{
    vec4 color;
    vec2 size;
} attribOut;

out vec3 vs_out_position_view;

void main()
{
    // TODO
    vec4 positionView = modelView * vec4(position, 1.0);
    vs_out_position_view = positionView.xyz / positionView.w;

    attribOut.color = color;
    attribOut.size = size;
}
