#version 450
layout (location = 0) in vec3 aPos;

layout(set = 0, binding = 0) uniform CameraVectors {
	mat4 view;
	mat4 projection;
} cameraData;

layout(location = 0) out vec3 TexCoords;



void main() {
	TexCoords = aPos;
	//TexCoords.xy *= -1;
	mat4 viewMatrix = mat4(mat3(cameraData.view));
    gl_Position = cameraData.projection * viewMatrix * vec4(aPos, 1.0);
}