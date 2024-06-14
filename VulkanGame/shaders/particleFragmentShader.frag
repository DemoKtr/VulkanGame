#version 450


layout (location = 0) in vec2 outVel;
layout (location = 1) in float outGradientPos;

layout (location = 0) out vec4 outFragColor;

void main () 
{
	
	float col = outGradientPos;
	outFragColor = vec4(col*2.0f,col,0.0f,1.0f);
}