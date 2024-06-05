#version 450

layout(set = 0,binding = 0) uniform UBO {
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	vec4 heightScale;
	vec4 camPos;
} cameraData;

layout(std140,set = 0, binding = 1) readonly buffer storageBuffer {
	mat4 model[];
} ObjectData;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexNormal;
layout(location = 4) in vec3 vertexTangent;



layout(location = 0) out VS_OUT{
	vec3 TangentFragPos;
	vec2 TexCoords;
	vec3 Normal;
	float heightScale;
	vec3 TangentViewPos;
	vec3 T;
	vec3 N;
} vs_out;




void main() {

	vec3 WorldPos = vec3(ObjectData.model[gl_InstanceIndex] * vec4(vertexPosition,1.0f));
	vs_out.TexCoords = vertexTexCoord;
	vs_out.heightScale = cameraData.heightScale.x;
	vec3 T = normalize(mat3(ObjectData.model[gl_InstanceIndex]) * vertexTangent);
	vec3 N = normalize(mat3(ObjectData.model[gl_InstanceIndex]) * vertexNormal);
	T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T,B,N));
	vs_out.TangentFragPos = TBN * WorldPos;
	vs_out.TangentViewPos = TBN * vec3(cameraData.camPos.xyz);
	vs_out.T = T;
	vs_out.N = N;
	gl_Position = cameraData.viewProjection * ObjectData.model[gl_InstanceIndex] * vec4(vertexPosition, 1.0);
}