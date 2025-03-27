#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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


float computeSpotFactor(vec3 lightDirection, vec3 spotDirectionView, float spotAngleCosine, float spotInnerAngleCosine)
{
    float spotDotProduct = dot(-lightDirection, spotDirectionView);
    return (spotDotProduct < spotAngleCosine) ? 0.0 : mix(pow(spotDotProduct, spotExponent), smoothstep(spotAngleCosine, spotInnerAngleCosine, spotDotProduct), float(useDirect3D));
}

vec3 computeSpecular(vec3 lightDirection, vec3 viewDirection, vec3 normal, vec3 lightSpecular)
{
    float specularFactor = useBlinn ? dot(normalize(lightDirection + viewDirection), normal) : dot(reflect(-lightDirection, normal), viewDirection);
    return mat.specular * lightSpecular * pow(max(specularFactor, 0.0), mat.shininess);
}

void computeLighting(vec3 transformedPosition, vec3 viewDirection, vec3 transformedNormal, out vec3 computedAmbient, out vec3 computedDiffuse, out vec3 computedSpecular)
{
    computedAmbient = mat.ambient * (lightModelAmbient + lights[0].ambient + lights[1].ambient + lights[2].ambient);
    computedDiffuse = vec3(0.0);
    computedSpecular = vec3(0.0);

    float spotAngleCosine = cos(radians(spotOpeningAngle));
    float spotInnerAngleCosine = min(spotAngleCosine * 2.0, 1.0);

    for (int i = 0; i < 3; i++) {
        vec3 lightPositionInView = (view * vec4(lights[i].position, 1.0)).xyz;
        vec3 lightDirection = normalize(lightPositionInView - transformedPosition);
        float normalDotLight = max(dot(transformedNormal, lightDirection), 0.0);

        float spotFactor = useSpotlight ? computeSpotFactor(lightDirection, normalize((view * vec4(lights[i].spotDirection, 0.0)).xyz), spotAngleCosine, spotInnerAngleCosine) : 1.0;

        computedDiffuse += mat.diffuse * lights[i].diffuse * normalDotLight * spotFactor;
        if (normalDotLight > 0.0) {
            computedSpecular += computeSpecular(lightDirection, viewDirection, transformedNormal, lights[i].specular) * spotFactor;
        }
    }
}

void main()
{
    vec3 transformedPosition = (modelView * vec4(position, 1)).xyz;
    vec3 viewDirection = normalize(-transformedPosition);
    vec3 transformedNormal = normalize(normalMatrix * normal);

    vec3 computedAmbient, computedDiffuse, computedSpecular;
    computeLighting(transformedPosition, viewDirection, transformedNormal, computedAmbient, computedDiffuse, computedSpecular);

    gl_Position = mvp * vec4(position, 1);
    attribOut.texCoords = texCoords;
    attribOut.ambient = computedAmbient;
    attribOut.diffuse = computedDiffuse;
    attribOut.specular = computedSpecular;
    attribOut.emission = mat.emission;
}