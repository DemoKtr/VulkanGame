#version 450

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inGradientPos;

layout (location = 0) out vec2 outVel;
layout (location = 1) out float outGradientPosX;
layout (location = 2) out float outGradientPosY;




layout(set = 0,binding = 0) uniform UBOParticle {
	mat4 model;
	mat4 view;
	mat4 projection;
} cameraData;



out gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
};

void main () 
{
  gl_PointSize = 1.0f;

	
	gl_Position = cameraData.projection * cameraData.view * cameraData.model *vec4(inPos.xy,inGradientPos.z ,1.0);
  outVel = inPos.zw;
  outGradientPosX = inGradientPos.x;
  outGradientPosY = inGradientPos.y;
  
}