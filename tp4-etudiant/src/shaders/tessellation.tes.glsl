#version 400 core

layout(quads) in;

/*
in Attribs {
    vec4 couleur;
} AttribsIn[];*/


out ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribOut;

uniform mat4 mvp;

uniform sampler2D heighmapSampler;

vec4 interpole( vec4 v0, vec4 v1, vec4 v2, vec4 v3 )
{
    // mix( x, y, f ) = x * (1-f) + y * f.
    // TODO
    vec4 v01 = mix(v0, v1, gl_TessCoord.x);
    vec4 v32 = mix(v3, v2, gl_TessCoord.x);
    return mix(v01, v32, gl_TessCoord.y);
}


const float PLANE_SIZE = 256.0f;

void main()
{
	// TODO
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;
    vec4 position = interpole(p0, p1, p2, p3);

    vec2 texCoordHeight = (position.xz + vec2(PLANE_SIZE * 0.5)) / (PLANE_SIZE * 4.0);

    float texel = texture(heighmapSampler, texCoordHeight).r;
    float height = texel * 64 - 32;
    position.y = height;
    attribOut.height = texel;

    gl_Position = mvp * position;

    attribOut.patchDistance = vec4(gl_TessCoord.xy, 1.0 - gl_TessCoord.xy);
    attribOut.texCoords = gl_TessCoord.xy * 2.0;


}
