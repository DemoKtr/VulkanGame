#version 450

layout(location = 0) in VS_OUT{
	vec2 TexCoords;
} fs_in;

layout(location = 0) out vec4 finalColor;

layout(set=1,binding=0) uniform sampler2D albedo;
layout(set=1,binding=1) uniform sampler2D normalMap;
layout(set=1,binding=2) uniform sampler2D armMap;
layout(set=1,binding=3) uniform sampler2D depthMap;


void main() {

    vec2 texCoords = fs_in.TexCoords;
	vec4 color = texture(albedo,texCoords);
	color.w = gl_FragCoord.z;
	finalColor = vec4(0.0f,0.4f,0.6f,1.0f);
	
}