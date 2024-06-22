#version 450


layout(set=0,binding=0) uniform sampler2D gPos;
layout(set=0,binding=1) uniform sampler2D gNormal;
layout(set=0,binding=2) uniform sampler2D texNoise;


layout(set = 0,binding = 3) uniform ssaoUBO {
	mat4 projection;
	vec3 samples[64];
	vec2 screenSize;
	int kernelSize;//= 64;
	float radius;// = 0.5;
	float bias;// = 0.025;
} ssaoSamples;


layout (location = 0) in vec2 inUV;

layout(location = 3) out vec4 gAlbedoSpec;


void main() 
{
	vec2 noiseScale = ssaoSamples.screenSize / 4.0f; 
	vec3 fragPos = texture(gPos, inUV).xyz;
	vec3 fragNormal = normalize(texture(gNormal, inUV).rgb);
	vec3 randomVec = normalize(texture(texNoise, inUV * noiseScale).xyz);
	vec3 tangent = normalize(randomVec - fragNormal * dot(randomVec, fragNormal));
    vec3 bitangent = cross(fragNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, fragNormal);
	float occlusion = 0.0;
    for(int i = 0; i < ssaoSamples.kernelSize; ++i)
    {
        vec3 samplePos = TBN * ssaoSamples.samples[i]; 
        samplePos = fragPos + samplePos * ssaoSamples.radius; 
        vec4 offset = vec4(samplePos, 1.0);
        offset = ssaoSamples.projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(gPos, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, ssaoSamples.radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + ssaoSamples.bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / ssaoSamples.kernelSize);
    
    gAlbedoSpec.a = occlusion;
}