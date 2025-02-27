#version 330 core

// TODO

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 mat;
uniform int textureIndex;
uniform int isPlate;

out vec2 TexCoord;

void main()
{
    gl_Position = mat * vec4(aPos, 1.0);

    if (isPlate == 1) {
        vec2 atlasSize = vec2(1.0 / 6.0, 1.0 / 4.0); 

        vec2 baseOffset = vec2(2.0 / 3.0, 0.5);

        vec2 offset = baseOffset + vec2((textureIndex % 2), (textureIndex % 2));
        TexCoord = (aTexCoord + offset) * atlasSize;
    } else {
            vec2 atlasSize = vec2(1.0 / 3.0, 0.5); 
            vec2 offset = vec2(float(textureIndex % 2), float(textureIndex /2));
            TexCoord = (aTexCoord + offset) * atlasSize;
    }
}