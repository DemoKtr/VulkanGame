#version 450

layout (location = 0) in vec4 FragPos;



layout(set = 0,binding = 1) uniform data {
	vec3 lightPos;
	float far_plane;
} lightData;

layout (location = 5) out float outColor;

void main() {
	float lightDistance = length(FragPos.xyz - lightData.lightPos);
    
    // map to [0;1] range by dividing by far_plane
    outColor = lightDistance / lightData.far_plane;
    

}