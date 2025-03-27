#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in ATTRIB_OUT
{
    vec3 position;
    vec2 texCoords;
} attribIn[];

out ATTRIB_VS_OUT
{
    vec2 texCoords;    
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} attribOut;

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


//spotlight OpenGL ou Direct3D
float computeSpotFactor(vec3 lightDirection, vec3 spotDirection, float spotAngleCosine)
{
    vec3 L = normalize(lightDirection);
    vec3 D = normalize(spotDirection);
    
    float spotDotProduct = dot(-L, D);
    
    if (spotDotProduct < spotAngleCosine) {
        return 0.0;
    }
    
    //Atténuation
    if (useDirect3D) {
        float cosOuter = spotAngleCosine;
        float cosInner = pow(spotAngleCosine, 1.01 + spotExponent/2.0);
        
        if (cosOuter > cosInner) {
            float temp = cosOuter;
            cosOuter = cosInner;
            cosInner = temp;
        }
        
        return smoothstep(cosOuter, cosInner, spotDotProduct);
    } else {
        return pow(spotDotProduct, spotExponent);
    }
}


// Calcule la composante spéculaire selon le modèle de Phong ou Blinn
vec3 computeSpecular(vec3 lightDirection, vec3 viewDirection, vec3 normal, vec3 lightSpecular)
{
    float specularFactor;
    
    if (useBlinn) {
        //Blinn
        vec3 halfwayDir = normalize(lightDirection + viewDirection);
        specularFactor = max(dot(normal, halfwayDir), 0.0);
    } else {
        //Phong
        vec3 reflectDir = reflect(-lightDirection, normal);
        specularFactor = max(dot(viewDirection, reflectDir), 0.0);
    }
    
    return mat.specular * lightSpecular * pow(specularFactor, mat.shininess);
}

void main()
{
    //normale de faces
    vec3 side1 = attribIn[1].position - attribIn[0].position;
    vec3 side2 = attribIn[2].position - attribIn[0].position;
    vec3 normal = normalize(cross(side1, side2));
    
    vec3 faceCenter = (attribIn[0].position + attribIn[1].position + attribIn[2].position) / 3.0;
    vec3 viewDirection = normalize(-faceCenter);
    vec3 ambient = mat.ambient * lightModelAmbient;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    
    float spotAngleCosine = cos(radians(spotOpeningAngle));
    
    for (int i = 0; i < 3; i++) {
        vec3 lightPositionView = (view * vec4(lights[i].position, 1.0)).xyz;
        vec3 lightDirection = normalize(lightPositionView - faceCenter);
        float diffuseFactor = max(dot(normal, lightDirection), 0.0);
        
        float spotFactor = 1.0;
        if (useSpotlight) {
            vec3 spotDirectionView = normalize((view * vec4(lights[i].spotDirection, 0.0)).xyz);
            spotFactor = computeSpotFactor(lightDirection, spotDirectionView, spotAngleCosine);
        }
        
        ambient += mat.ambient * lights[i].ambient;
        diffuse += mat.diffuse * lights[i].diffuse * diffuseFactor * spotFactor;
        
        if (diffuseFactor > 0.0) {
            specular += computeSpecular(lightDirection, viewDirection, normal, lights[i].specular) * spotFactor;
        }
    }
    
    for (int i = 0; i < 3; i++) {
        attribOut.texCoords = attribIn[i].texCoords;
        attribOut.emission = mat.emission;
        attribOut.ambient = ambient;
        attribOut.diffuse = diffuse;
        attribOut.specular = specular;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    
    EndPrimitive();
}

