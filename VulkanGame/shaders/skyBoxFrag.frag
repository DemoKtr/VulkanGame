#version 450
layout(set = 1, binding = 0) uniform samplerCube material;
layout(set=0,binding=1) uniform sampler2D albedoMap;
layout(set=0,binding=2) uniform sampler2D particleMap;


layout(location = 0) out vec4 outColor;


layout(location = 0) in vec2 inUV;
layout(location = 1) in vec3 forwards;

void main() {
	vec3 color = vec3 (0.0f,0.0f,0.0f);
	float zAlbedo = texture(albedoMap,inUV.xy).a;
	float zParticle = texture(particleMap,inUV.xy).a;
	if(zAlbedo < 1.0f && zParticle < 1.0f){
		if(zParticle > zAlbedo) color = texture(albedoMap,inUV.xy).rgb;
		else color = texture(particleMap,inUV.xy).rgb;
	}
	else color = texture(material, forwards).rgb;
	outColor = vec4(color,1.0f);
	//outColor = vec4(0.0, 1.0, 0.0, 1.0);
}