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

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

layout(set=1,binding=0) uniform sampler2D material;
layout(set=1,binding=1) uniform sampler2D normalMap;


void main() {
	vec3 normal =  texture(normalMap,fragTexCoord).rgb;
	//outColor = vec4(fragColor, 1.0) * texture(material,fragTexCoord);
	gPosition = outPos;
	gNormal = normalize(outNormalMatrix*normal);
	gAlbedoSpec.rgb = texture(material,fragTexCoord).rgb;
	gAlbedoSpec.a = 1.0f;
	
}