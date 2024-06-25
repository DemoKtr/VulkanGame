#version 450
layout(set=0,binding=0) uniform sampler2D finalImage;

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 inUV;


void main() {

	
	vec3 color = texture(finalImage,inUV.xy).rgb;


	outColor = vec4(color,1.0f);
}