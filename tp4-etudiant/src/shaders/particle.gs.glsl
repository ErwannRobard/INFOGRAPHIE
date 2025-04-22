#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


in ATTRIB_VS_OUT
{
    vec4 color;
    vec2 size;
} attribIn[];

in vec3 vs_out_position_view[];

out ATTRIB_GS_OUT
{
    vec4 color;
    vec2 texCoords;
} attribOut;

uniform mat4 projection;

void main()
{
    // TODO
    //billboard quad gen
    vec3 positionView = vs_out_position_view[0];
    vec4 color = attribIn[0].color;
    vec2 size = attribIn[0].size;

    //billboard quad offsets in view space 
    vec2 offsets[4] = vec2[](
        vec2(-0.5, -0.5), //BL
        vec2( 0.5, -0.5), //BR
        vec2(-0.5,  0.5), //TL
        vec2( 0.5,  0.5)  //TR
    );

    //define texture coordinates
    vec2 texCoords[4] = vec2[](
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(0.0, 1.0),
        vec2(1.0, 1.0)
    );

    //emit the 4 vertices of the quad
    for (int i = 0; i < 4; i++)
    {
        vec3 vertexPosView = positionView + vec3(offsets[i] * size, 0.0);

        //N’oublier pas la matrice de projection
        gl_Position = projection * vec4(vertexPosView, 1.0);

        //Les coordonnées de textures sont aussi générées
        attribOut.color = color;
        attribOut.texCoords = texCoords[i];

        EmitVertex();
    }

    EndPrimitive();

}
