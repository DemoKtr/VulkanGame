#version 450
layout(location = 0) in vec3 vertexPosition;

layout(std140,set = 0, binding = 1) readonly buffer storageBuffer {
	mat4 model[];
} ObjectData;


void main() {
	gl_Position = ObjectData.model[gl_InstanceIndex] * vec4(vertexPosition, 1.0);
}