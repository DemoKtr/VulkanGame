#version 450

layout (input_attachment_index = 0, binding = 0) uniform subpassInput inputPosition;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput inputNormal;
layout (input_attachment_index = 2, binding = 2) uniform subpassInput inputAlbedo;
layout (input_attachment_index = 3, binding = 3) uniform subpassInput inputARM;


layout(set = 0,binding = 4) uniform PointLight{
		vec3 position;

		float constant;
		float linear;
		float quadratic;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
} light;


layout (constant_id = 0) const float NEAR_PLANE = 0.1f;
layout (constant_id = 1) const float FAR_PLANE = 256.0f;
layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;


void main() {
	vec3 fragPos = subpassLoad(inputPosition).rgb;
	vec3 normal = subpassLoad(inputNormal).rgb;
	vec4 albedo = subpassLoad(inputAlbedo);
	#define ambient 0.05;
		

	outColor = albedo;
}