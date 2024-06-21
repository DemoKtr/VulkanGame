#version 450
layout(set=1,binding=0) uniform samplerCube skybox;



layout(location = 0) in vec3 TexCoords;
layout(location = 0) out vec4 FragColor;

void main() {
	FragColor = texture(skybox, TexCoords);
}