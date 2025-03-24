// TODO
#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D colorTexture;

void main()
{
    fragColor = texture(colorTexture, fragTexCoord);
}
