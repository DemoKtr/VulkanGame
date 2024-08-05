#version 450
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
layout(set = 0,binding = 0) uniform UBO {
	mat4 view;
	mat4 projection;
} cameraData;

struct SBO {
    mat4 model;
    mat4 finalBonesMatrices[MAX_BONES];
};


layout(std140, set = 0, binding = 1) readonly buffer StorageBuffer {
    SBO sbo[];
} ObjectData;

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexTexCoord;
layout(location = 2) in vec4 vertexNormal;
layout(location = 3) in vec4 tangent;
layout(location = 4) in vec4 bitangent;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 weights;





layout(location = 0) out VS_OUT{
	vec2 TexCoords;
} vs_out;




void main() {

	vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; ++i)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(vertexPosition);
            break;
        }
        vec4 localPosition = ObjectData.sbo[gl_InstanceIndex].finalBonesMatrices[boneIds[i]] * vec4(vertexPosition);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(ObjectData.sbo[gl_InstanceIndex].finalBonesMatrices[boneIds[i]]) * vec3(vertexNormal);
   }
	
    mat4 viewModel = cameraData.view * ObjectData.sbo[gl_InstanceIndex].model;
    gl_Position =  cameraData.projection * viewModel * totalPosition;
	vs_out.TexCoords = vertexTexCoord.xy;


}