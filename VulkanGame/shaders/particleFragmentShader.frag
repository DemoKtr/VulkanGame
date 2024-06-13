#version 450


layout (location = 0) in vec2 outVel;
layout (location = 1) in vec4 outCol;

layout (location = 0) out vec4 outFragColor;

void main () 
{
	
	outFragColor = vec4(0.0f,0.0f,1.0f,1.0f);
}