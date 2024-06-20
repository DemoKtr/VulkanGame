#version 450

layout(location = 0) in VS_OUT{
	vec3 TangentFragPos;
	vec2 TexCoords;
	vec3 Normal;
	float heightScale;
	vec3 TangentViewPos;
	vec3 T;
	vec3 N;
	vec3 worldPos;
	float distanceToCamera;
} fs_in;


layout(location = 0) out vec4 gColor;
layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;
layout(location = 3) out vec4 gAlbedoSpec;
layout(location = 4) out vec4 gARM;
layout(location = 5) out vec4 gT;
layout(location = 6) out vec4 gWP;

layout(set=1,binding=0) uniform sampler2D albedo;
layout(set=1,binding=1) uniform sampler2D normalMap;
layout(set=1,binding=2) uniform sampler2D armMap;
layout(set=1,binding=3) uniform sampler2D depthMap;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
   float layerDepth = 1.0 / 48.0f;
	float currLayerDepth = 0.0;
	vec2 deltaUV = viewDir.xy * 0.01 / (viewDir.z * 48.0f);
	vec2 currUV = texCoords;
	float height = 1.0 - textureLod(depthMap, currUV, 0.0).a;
	for (int i = 0; i < 48.0f; i++) {
		currLayerDepth += layerDepth;
		currUV -= deltaUV;
		height = 1.0 - textureLod(depthMap, currUV, 0.0).a;
		if (height < currLayerDepth) {
			break;
		}
	}
	vec2 prevUV = currUV + deltaUV;
	float nextDepth = height - currLayerDepth;
	float prevDepth = 1.0 - textureLod(depthMap, prevUV, 0.0).a - currLayerDepth + layerDepth;
	return mix(currUV, prevUV, nextDepth / (nextDepth - prevDepth));
}

void main() {
	gWP.rgb = fs_in.worldPos;
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
	vec3 normal = texture(normalMap,texCoords).rgb;
	texCoords = (ParallaxMapping(fs_in.TexCoords,  viewDir));  
	
	normal = normalize(normal*2.0f-1.0f);
	gPosition = vec4(fs_in.TangentFragPos, gl_FragCoord.z);
	gNormal.rgb = normal;
	vec3 arm = texture(armMap,texCoords).rgb;
	gAlbedoSpec.rgb =pow((texture(albedo,texCoords).rgb),vec3(2.2f));
	gAlbedoSpec.a = 1.0f;
	gARM.rgb = arm;
	gT.rgb = fs_in.T;
	gNormal.a = fs_in.N.x;
	gARM.a = fs_in.N.y;
	gT.a = fs_in.N.z;	
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		discard;
	
}