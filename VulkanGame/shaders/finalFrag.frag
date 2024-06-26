#version 450
layout(set=0,binding=0) uniform sampler2D finalImage;
layout(set=0,binding=1) uniform sampler2D bloomImage;

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 inUV;


void main() {

	
	vec3 result = vec3(0.0);

	vec3 hdrColor = texture(finalImage,inUV.xy).rgb;
    vec3 bloomColor = texture(bloomImage, inUV.xy).rgb;
     result = mix(hdrColor, bloomColor,0.04f);
	
	result = vec3(1.0) - exp(-result * 1);
	//result = result / (result + vec3(1.0));

    result = pow(result, vec3(1.0 / 2.2f));
	
	outColor = vec4(result,1.0f);
}