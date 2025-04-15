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
const vec3 PATCH_EDGE_COLOR = vec3(1.0f, 0.0f, 0.0f);

const float WIREFRAME_WIDTH = 0.5f;
const float PATCH_EDGE_WIDTH = 0.5f;

void main()
{
	// TODO
    float height = attribIn.height;

    float sandFactor  = smoothstep(0.0, 0.3, height);
    float grassFactor = smoothstep(0.35, 0.6, height);
    float snowFactor  = smoothstep(0.65, 1.0, height);

    vec2 uv = attribIn.texCoords * 2.0;
    vec4 sandTex  = texture(sandSampler, uv);
    vec4 grassTex = texture(groundSampler, uv);
    vec4 snowTex  = texture(snowSampler, uv);

    vec4 sandGrassTex = mix(sandTex, grassTex, smoothstep(0.3, 0.35, height));
    vec4 grassSnowTex = mix(grassTex, snowTex, smoothstep(0.6, 0.65, height));

    vec4 baseColor = mix(sandTex, sandGrassTex, clamp(sandFactor - grassFactor, 0.0, 1.0));
    baseColor = mix(baseColor, grassTex, clamp(grassFactor, 0.0, 1.0));
    baseColor = mix(baseColor, grassSnowTex, clamp(grassFactor - snowFactor, 0.0, 1.0));
    baseColor = mix(baseColor, snowTex, clamp(snowFactor, 0.0, 1.0));
    if (viewWireframe) {
        float baryFactor = edgeFactor(attribIn.barycentricCoords, WIREFRAME_WIDTH);
        vec3 finalColor = mix(WIREFRAME_COLOR, baseColor.rgb, 1.0 - baryFactor);
        
        float patchFactor = edgeFactor(attribIn.patchDistance, PATCH_EDGE_WIDTH);
        finalColor = mix(PATCH_EDGE_COLOR, finalColor, 1.0 - patchFactor);
        FragColor = vec4(finalColor, 1.0);
    } else {
        FragColor = baseColor;
    }
}
