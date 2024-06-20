#version 450

layout (set=1,binding=0) uniform sampler2D samplerColorMap;
layout (set=1,binding=1) uniform sampler2D samplerGradientRamp;


layout (location = 0) in vec2 outVel;
layout (location = 1) in float outGradientPosX;
layout (location = 2) in float outGradientPosY;



layout (location = 0) out vec4 outFragColor;

void main () 
{
	vec2 texCoords2 = vec2(outGradientPosX,outGradientPosY);
	vec4 col = texture(samplerGradientRamp, texCoords2);
	vec4 color = texture(samplerGradientRamp, texCoords2);
    outFragColor = vec4(col.rgb, gl_FragCoord.z);
}