#version 450



layout (location = 0) in vec4 inColor;
layout (location = 1) in float inGradientPos;

layout (location = 0) out vec4 outFragColor;

void main () 
{
	vec3 color = vec3(1.0f,0.0,0.0f);
	outFragColor.rgb = vec3(1.0f,0.0,0.0f);
}