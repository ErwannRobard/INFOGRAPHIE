#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} attribIn;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

void main()
{
    // TODO
    
    vec3 colour = (attribIn.ambient + attribIn.diffuse) * texture(diffuseSampler, attribIn.texCoords).rgb 
                 + attribIn.emission + (attribIn.specular * texture(specularSampler, attribIn.texCoords).r);

    FragColor = vec4(colour, 1.0);

}
