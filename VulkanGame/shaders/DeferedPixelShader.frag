#version 450

layout (input_attachment_index = 0, binding = 0) uniform subpassInput inputPosition;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput inputNormal;
layout (input_attachment_index = 2, binding = 2) uniform subpassInput inputAlbedo;

layout (location = 0) out vec4 outColor;

void main() {
	vec3 fragPos = subpassLoad(inputPosition).rgb;
	vec3 normal = subpassLoad(inputNormal).rgb;
	vec4 albedo = subpassLoad(inputAlbedo);
	#define ambient 0.05
	
	// Ambient part
	vec3 fragcolor  = albedo.rgb * ambient;
	outColor = vec4(fragcolor, 1.0);
}