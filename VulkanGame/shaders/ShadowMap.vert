#version 450

layout (location = 0) in vec3 inPos;

layout (location = 0) out int outInstanceIndex;

layout(std140,set = 0, binding = 0) readonly buffer storageBuffer {
	mat4 model[];
} ObjectData;

void main()
{
	outInstanceIndex = gl_InstanceIndex;
	gl_Position = ObjectData.model[gl_InstanceIndex] * vec4(inPos,1.0f);
}