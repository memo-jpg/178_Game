#version 120

varying vec2 v_texcoord;	// share with fragment shader
varying vec4 v_color;

void main(){

	v_texcoord = gl_MuliTexCoord0.xy;	// grab texture coordinates from cpu
	
	// grab color values
	v_color = gl_Color;

	// grab model view projection matrix

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}