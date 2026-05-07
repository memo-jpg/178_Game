#version 120

varying vec2 v_texcoord;

void main()
{
    v_texcoord = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
