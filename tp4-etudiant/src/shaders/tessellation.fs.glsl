#version 410 core

in ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribIn;

uniform sampler2D groundSampler;
uniform sampler2D sandSampler;
uniform sampler2D snowSampler;
uniform bool viewWireframe;

out vec4 FragColor;

float edgeFactor(vec3 barycentricCoords, float width)
{
    vec3 d = fwidth(barycentricCoords);
    vec3 f = step(d * width, barycentricCoords);
    return min(min(f.x, f.y), f.z);
}

float edgeFactor(vec4 barycentricCoords, float width)
{
    vec4 d = fwidth(barycentricCoords);
    vec4 f = step(d * width, barycentricCoords);
    return min(min(min(f.x, f.y), f.z), f.w);
}

const vec3 WIREFRAME_COLOR = vec3(0.5f);
const vec3 PATCH_EDGE_COLOR = vec3(0.9f, 0.1f, 0.1f);

const float WIREFRAME_WIDTH = 0.5f;
const float PATCH_EDGE_WIDTH = 0.5f;

void main()
{
	// TODO
    float height = attribIn.height;

    float sandGrassFactor  = smoothstep(0.35, 0.4, height);
    float sandGrassSnowFactor = smoothstep(0.6, 0.65, height);
    
    vec4 sandTex  = texture(sandSampler, attribIn.texCoords);
    vec4 grassTex = texture(groundSampler, attribIn.texCoords);
    vec4 snowTex  = texture(snowSampler, attribIn.texCoords);

    vec4 sandGrassTex = mix(sandTex, grassTex, sandGrassFactor);
    vec4 sandGrassSnowTex = mix(sandGrassTex, snowTex, sandGrassSnowFactor);

    vec3 baseColor = sandGrassSnowTex.rgb;
    if (viewWireframe) {
        float baryFactor = edgeFactor(attribIn.barycentricCoords, WIREFRAME_WIDTH);
        vec3 finalColor = mix(WIREFRAME_COLOR, baseColor, baryFactor);
        
        float patchFactor = edgeFactor(attribIn.patchDistance, PATCH_EDGE_WIDTH);
        finalColor = mix(PATCH_EDGE_COLOR, finalColor, patchFactor);
        FragColor = vec4(finalColor, 1.0);
    } else {
        FragColor = vec4(baseColor, 1.0);
    }
}
