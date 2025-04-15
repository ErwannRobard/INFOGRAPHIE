#version 400 core

layout(vertices = 4) out;

uniform mat4 modelView;


const float MIN_TESS = 4;
const float MAX_TESS = 64;

const float MIN_DIST = 30.0f;
const float MAX_DIST = 100.0f;

void main()
{
	// TODO

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    vec3 centre0 = (vec3(modelView * gl_in[0].gl_Position) + vec3(modelView * gl_in[3].gl_Position)) * 0.5;
    vec3 centre1 = (vec3(modelView * gl_in[0].gl_Position) + vec3(modelView * gl_in[1].gl_Position)) * 0.5;
    vec3 centre2 = (vec3(modelView * gl_in[2].gl_Position) + vec3(modelView * gl_in[1].gl_Position)) * 0.5;
    vec3 centre3 = (vec3(modelView * gl_in[3].gl_Position) + vec3(modelView * gl_in[2].gl_Position)) * 0.5;

    float distance0 = length(centre0);
    float distance1 = length(centre1);
    float distance2 = length(centre2);
    float distance3 = length(centre3);

    float mix0 = smoothstep(MIN_DIST, MAX_DIST, clamp(distance0, MIN_DIST, MAX_DIST));
    float mix1 = smoothstep(MIN_DIST, MAX_DIST, clamp(distance1, MIN_DIST, MAX_DIST));
    float mix2 = smoothstep(MIN_DIST, MAX_DIST, clamp(distance2, MIN_DIST, MAX_DIST));
    float mix3 = smoothstep(MIN_DIST, MAX_DIST, clamp(distance3, MIN_DIST, MAX_DIST));

    gl_TessLevelOuter[0] = mix(MAX_TESS, MIN_TESS, 1 - mix0);
    gl_TessLevelOuter[1] = mix(MAX_TESS, MIN_TESS, 1 - mix1);
    gl_TessLevelOuter[2] = mix(MAX_TESS, MIN_TESS, 1 - mix2);
    gl_TessLevelOuter[3] = mix(MAX_TESS, MIN_TESS, 1 - mix3);

    gl_TessLevelInner[0] = max(gl_TessLevelOuter[0], gl_TessLevelOuter[3]);
    gl_TessLevelInner[1] = max(gl_TessLevelOuter[0], gl_TessLevelOuter[2]);
}
