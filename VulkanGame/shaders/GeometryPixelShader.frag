#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 outPos;
layout(location = 3) in mat3 outNormalMatrix;

layout(set = 0,binding = 2) uniform PointLight{
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

layout(location = 0) out vec4 gColor;
layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;
layout(location = 3) out vec4 gAlbedoSpec;

layout(set=1,binding=0) uniform sampler2D material;
layout(set=1,binding=1) uniform sampler2D normalMap;

float linearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f; 
	return (2.0f * NEAR_PLANE * FAR_PLANE) / (FAR_PLANE + NEAR_PLANE - z * (FAR_PLANE - NEAR_PLANE));	
}

void main() {

	vec3 normal =  texture(normalMap,fragTexCoord).rgb;
	gPosition = vec4(outPos,1.0f);
	gPosition.a = linearDepth(gl_FragCoord.z);
	vec3 N = normalize(outNormalMatrix*normal);
	N.y = -N.y;
	gAlbedoSpec.rgb = texture(material,fragTexCoord).rgb;
	gAlbedoSpec.a = 1.0f;
	gNormal = vec4(N,1.0f);

	
}