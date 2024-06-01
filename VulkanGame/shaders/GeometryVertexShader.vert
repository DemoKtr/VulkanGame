#version 450

layout(set = 0,binding = 0) uniform UBO {
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	vec3 lightPos[2];
	vec3 viewPos;
	float heightScale;
} cameraData;

layout(std140,set = 0, binding = 1) readonly buffer storageBuffer {
	mat4 model[];
} ObjectData;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;


layout(location = 0) out VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos[2];
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	float heightScale;
} vs_out;




void main() {

	vs_out.FragPos = vec3(ObjectData.model[gl_InstanceIndex] * vec4(vertexPosition,1.0f));
	vs_out.TexCoords = vertexTexCoord;
	
	mat3 normalMatrix = transpose(inverse(mat3(ObjectData.model[gl_InstanceIndex])));
	vec3 T = normalize(normalMatrix * vertexTangent);
	vec3 N = normalize(normalMatrix * vertexNormal);
	T = normalize(T - dot(T,N) * N);
	vec3 B = cross(N,T);
	
	mat3 TBN = transpose(mat3(T,B,N));
	for(int i =0; i<2;++i) vs_out.TangentLightPos[i] = TBN * cameraData.lightPos[i];
	vs_out.TangentViewPos = TBN * cameraData.viewPos;
	vs_out.TangentFragPos = TBN * vs_out.FragPos;
	vs_out.heightScale = cameraData.heightScale;
	gl_Position = cameraData.viewProjection * ObjectData.model[gl_InstanceIndex] * vec4(vertexPosition, 1.0);
}