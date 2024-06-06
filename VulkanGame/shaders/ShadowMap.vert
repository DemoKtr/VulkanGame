#version 450

layout (location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexNormal;
layout(location = 4) in vec3 vertexTangent;

layout(location = 0) out int outInstanceIndex;
layout(location = 1) out mat4 modelMatrix;
layout(location = 5) out mat3 tbnMatrix;





layout(std140,set = 0, binding = 0) readonly buffer storageBuffer {
	mat4 model[];
} ObjectData;

void main()
{
	outInstanceIndex = gl_InstanceIndex;
	modelMatrix= ObjectData.model[gl_InstanceIndex];
	vec3 T = normalize(mat3(ObjectData.model[gl_InstanceIndex]) * vertexTangent);
	vec3 N = normalize(mat3(ObjectData.model[gl_InstanceIndex]) * vertexNormal);
	T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
	tbnMatrix = transpose(mat3(T,B,N));
	gl_Position =vec4(vertexPosition,1.0f);
	
}