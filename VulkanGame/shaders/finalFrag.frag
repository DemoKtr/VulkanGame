#version 450
layout(set=0,binding=0) uniform sampler2D finalImage;

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 inUV;


void main() {

	
	vec3 color = texture(finalImage,inUV.xy).rgb;

	//color = color / (color + vec3(1.0));
    // gamma correct
    //color = pow(color, vec3(1.0/2.2));
	outColor = vec4(color,1.0f);
}