#version 330 core

in vec2 fragUV;
uniform sampler2D colorTexture;
out vec4 fragColor;

// TODO
void main()
{
    vec4 tex = texture(colorTexture, fragUV);
    vec3 blueTint = vec3(0.15, 0.4, 1.0);
    fragColor = vec4(tex.rgb * blueTint, tex.a);
}

