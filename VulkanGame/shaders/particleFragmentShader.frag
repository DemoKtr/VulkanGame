#version 450

layout (set=1,binding=0) uniform sampler2D samplerColorMap;
layout (set=1,binding=1) uniform sampler2D samplerGradientRamp;


layout (location = 0) in vec2 outVel;
layout (location = 1) in float outGradientPosX;
layout (location = 2) in float outGradientPosY;
layout (location = 3) in float outGradientPosZ;
layout (location = 4) in float outGradientPosW;


layout (location = 0) out vec4 outFragColor;

void main () 
{
	//vec4 color = vec4(texture(samplerGradientRamp, vec2(outGradientPosX, outGradientPosY)).rgb,1.0f);
	//outFragColor = vec4(texture(samplerColorMap, jakie coordynaty?))*color;
	  
	  
	vec2 texCoords = vec2(outGradientPosZ,outGradientPosW);
	vec2 texCoords2 = vec2(outGradientPosX,outGradientPosY);
	vec4 col = texture(samplerColorMap, texCoords);
	vec4 color = texture(samplerGradientRamp, texCoords2);
	if(col.r<0.05f || col.g<0.05f,col.b<0.05f) discard;
    // Pobierz kolor z tekstury
    outFragColor = vec4(col.xyz, gl_FragCoord.z);
}