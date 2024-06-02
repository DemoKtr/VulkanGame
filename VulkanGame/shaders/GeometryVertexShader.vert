#version 450

layout(set = 0,binding = 0) uniform UBO {
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	float heightScale;
} cameraData;

layout(std140,set = 0, binding = 1) readonly buffer storageBuffer {
	mat4 model[];
} ObjectData;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexNormal;



layout(location = 0) out VS_OUT{
	vec3 WorldPos;
	vec2 TexCoords;
	vec3 Normal;
	float heightScale;
} vs_out;




void main() {

	vs_out.WorldPos = vec3(ObjectData.model[gl_InstanceIndex] * vec4(vertexPosition,1.0f));
	vs_out.TexCoords = vertexTexCoord;
	vs_out.heightScale = cameraData.heightScale;
	mat3 normalMatrix = mat3(transpose(inverse(((ObjectData.model[gl_InstanceIndex])))));
	vs_out.Normal = normalMatrix *  vertexNormal;
	gl_Position = cameraData.viewProjection * ObjectData.model[gl_InstanceIndex] * vec4(vertexPosition, 1.0);
}