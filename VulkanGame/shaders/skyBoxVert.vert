#version 450
layout (location = 0) in vec3 aPos;

layout(location = 0) out vec3 TexCoords;

layout(set = 0,binding = 0) uniform UBOSkybox {
	mat4 view;
	mat4 projection;
} UBOData;

void main()
{
    TexCoords = aPos;
    vec4 pos = UBOData.projection * UBOData.view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  