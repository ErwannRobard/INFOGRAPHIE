#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
} attribOut;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 modelView;
uniform mat3 normalMatrix;

struct Material
{
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct UniversalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 spotDirection;
};

layout (std140) uniform LightingBlock
{
    Material mat;
    UniversalLight lights[3];
    vec3 lightModelAmbient;
    bool useBlinn;
    bool useSpotlight;
    bool useDirect3D;
    float spotExponent;
    float spotOpeningAngle;
};

void main()
{
    // TODO
    vec3 positionInView = (modelView * vec4(position, 1.0)).xyz;
    attribOut.normal = normalMatrix * normal;
    attribOut.obsPos = -positionInView;
    
    for (int i = 0; i < 3; i++) {
        vec3 lightPosInView = (view * vec4(lights[i].position, 1.0)).xyz;
        attribOut.lightDir[i] = lightPosInView - positionInView;
        attribOut.spotDir[i] = mat3(view) * lights[i].spotDirection;
    }
    
    attribOut.texCoords = texCoords;
    gl_Position = mvp * vec4(position, 1.0);
}
