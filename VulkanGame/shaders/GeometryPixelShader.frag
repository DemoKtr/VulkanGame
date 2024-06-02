#version 450

layout(location = 0) in VS_OUT{
	vec3 WorldPos;
	vec2 TexCoords;
	vec3 Normal;
	float heightScale;
} fs_in;


layout(location = 0) out vec4 gColor;
layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;
layout(location = 3) out vec4 gAlbedoSpec;
layout(location = 4) out vec4 gARM;

layout(set=1,binding=0) uniform sampler2D albedo;
layout(set=1,binding=1) uniform sampler2D normalMap;
layout(set=1,binding=2) uniform sampler2D armMap;
layout(set=1,binding=3) uniform sampler2D depthMap;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, fs_in.TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.WorldPos);
    vec3 Q2  = dFdy(fs_in.WorldPos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N   = normalize(fs_in.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main() {
	


    vec3 N = getNormalFromMap();
	gPosition = vec4(fs_in.WorldPos,1.0f);
	gNormal.rgb = N;
	vec3 arm = texture(armMap,fs_in.TexCoords).rgb;
	gAlbedoSpec.rgb =pow((texture(albedo,fs_in.TexCoords).rgb),vec3(2.2f));
	gAlbedoSpec.a = 1.0f;
	gARM.rgb = arm;
	
}