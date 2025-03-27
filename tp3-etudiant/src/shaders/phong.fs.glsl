#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
} attribIn;

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

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;


//Spot factor for OpenGl and Direct3D 
float computeSpotFactor(vec3 lightDirection, vec3 spotDirection, float spotAngleCosine)
{
    vec3 L = normalize(lightDirection);
    vec3 D = normalize(spotDirection);
    
    float spotDotProduct = dot(-L, D);
    
    if (spotDotProduct < spotAngleCosine) {
        return 0.0;
    }
    
    if (useDirect3D) {
        float cosInner = spotAngleCosine;
        float cosOuter = pow(spotAngleCosine, 1.01 + spotExponent/2.0);
        
        return smoothstep(cosOuter, cosInner, spotDotProduct);
    } else {
        return pow(spotDotProduct, spotExponent);
    }
}

void main()
{
    // TODO
    vec3 normal = normalize(attribIn.normal);
    vec3 viewDir = normalize(attribIn.obsPos);
    
    vec3 ambient = mat.ambient * lightModelAmbient;
    
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    float spotAngleCosine = cos(radians(spotOpeningAngle));
    
    for (int i = 0; i < 3; i++) {
        vec3 lightDir = normalize(attribIn.lightDir[i]);
        float diffuseFactor = max(dot(normal, lightDir), 0.0);
        float spotFactor = 1.0;
        if (useSpotlight) {
            spotFactor = computeSpotFactor(attribIn.lightDir[i], attribIn.spotDir[i], spotAngleCosine);
        }
        
        ambient += mat.ambient * lights[i].ambient;
        diffuse += mat.diffuse * lights[i].diffuse * diffuseFactor * spotFactor;
        if (diffuseFactor > 0.0) {
            float specularFactor;
            
            if (useBlinn) {
                //Blinn
                vec3 halfwayDir = normalize(lightDir + viewDir);
                specularFactor = pow(max(dot(normal, halfwayDir), 0.0), mat.shininess);
            } else {
                //Phong
                vec3 reflectDir = reflect(-lightDir, normal);
                specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
            }
            
            specular += mat.specular * lights[i].specular * specularFactor * spotFactor;
        }
    }
    
    vec3 diffuseMap = texture(diffuseSampler, attribIn.texCoords).rgb;
    float specularMap = texture(specularSampler, attribIn.texCoords).r;
    vec3 result = mat.emission + 
                 (ambient * diffuseMap) + 
                 (diffuse * diffuseMap) + 
                 (specular * specularMap);
    
    FragColor = vec4(result, 1.0);
}



