#version 450

layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec3 vertexCol;


layout(push_constant) uniform constants {
	mat4 model;
} ObjectData;

layout(location = 0) out vec3 fragColor;
void main() {
	gl_Position = ObjectData.model * vec4(vertexPos, 0.0, 1.0);
	fragColor = vertexCol;
}