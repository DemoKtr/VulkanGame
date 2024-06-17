#version 450

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inVel;
layout (location = 2) in vec4 inGradientPos;

layout (location = 0) out vec2 outVel;
layout (location = 1) out float outGradientPosX;
layout (location = 2) out float outGradientPosY;
layout (location = 3) out float outGradientPosZ;
layout (location = 4) out float outGradientPosW;



layout(set = 0,binding = 0) uniform UBO {
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	vec4 heightScale;
	vec4 camPos;
} cameraData;



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
  outGradientPosX = inGradientPos.x;
  outGradientPosY = inGradientPos.y;
  outGradientPosZ = inGradientPos.z;
  outGradientPosW = inGradientPos.w;
}