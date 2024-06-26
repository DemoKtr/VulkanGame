#version 450
layout(set=0,binding=0) uniform sampler2D skybox;
layout(set=0,binding=1) uniform sampler2D particleMap;
layout(set=0,binding=2) uniform sampler2D albedoMap;


layout(location = 0) out vec4 outColor;


layout(location = 0) in vec2 inUV;


void main() {

	vec3 color = vec3 (0.0f,0.0f,0.0f);
	float zAlbedo = texture(albedoMap,inUV.xy).a;
	float zParticle = texture(particleMap,inUV.xy).a;
	
	if(zAlbedo == 1.0f && zParticle == 1.0f){
	color = texture(skybox,inUV.xy).rgb;
	
	}else  
	{
	if(zParticle >= zAlbedo) color = texture(albedoMap,inUV.xy).rgb;
	else color =pow((texture(particleMap,inUV.xy).rgb),vec3(2.2f));
	}
	

	outColor = vec4(color,1.0f);
}