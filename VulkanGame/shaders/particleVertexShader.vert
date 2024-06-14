#version 450

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inVel;
layout (location = 2) in vec4 inGradientPos;

layout (location = 0) out vec2 outVel;
layout (location = 1) out float outGradientPos;

out gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
};

void main () 
{
  gl_PointSize = 8.0;

  gl_Position = vec4(inPos,0.0f ,1.0);
  outVel = inVel;
  outGradientPos = inGradientPos.x;
}