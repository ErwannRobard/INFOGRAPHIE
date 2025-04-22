#version 330 core

out vec4 FragColor;


uniform sampler2D textureSampler;

in ATTRIB_GS_OUT
{
    vec4 color;
    vec2 texCoords;
} attribIn;

void main()
{
    // TODO
    //sample flame texture
    vec4 texColor = texture(textureSampler, attribIn.texCoords);

    //discard lorsque l’alpha des texels est plus petit que 0.05
    if (texColor.a < 0.05) {
        discard;
    }

    //La couleur finale est le texel teinté par l’attribut couleur
    FragColor = texColor * attribIn.color;
}
