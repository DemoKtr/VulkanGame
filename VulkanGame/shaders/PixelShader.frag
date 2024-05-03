#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(set = 0,binding = 2) uniform PointLight{
		vec3 position;

		float constant;
		float linear;
		float quadratic;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
} light;

layout(location = 0) out vec4 outColor;

layout(set=1,binding=0) uniform sampler2D material;

void main() {
	
	outColor = vec4(fragColor, 1.0) * texture(material,fragTexCoord);
	if(light.constant <0.9f) outColor = vec4(0.0f);
}