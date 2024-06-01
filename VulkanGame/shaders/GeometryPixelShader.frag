#version 450

layout(location = 0) in VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos[2];
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	float heightScale;
} fs_in;


layout(location = 0) out vec4 gColor;
layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;
layout(location = 3) out vec4 gAlbedoSpec;
layout(location = 4) out vec4 gARM;

layout(set=1,binding=0) uniform sampler2D albedo;
layout(set=1,binding=1) uniform sampler2D normalMap;
layout(set=1,binding=2) uniform sampler2D armMap;
layout(set=1,binding=3) uniform sampler2D depthMap;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * fs_in.heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main() {
	
	
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
    
    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir); 

    vec3 normal = texture(normalMap,texCoords).rgb;	
	gPosition = vec4(fs_in.FragPos,1.0f);
	gNormal.rgb = normal;
	vec3 arm = texture(armMap,texCoords).rgb;
	gAlbedoSpec.rgb = 2.0f *(texture(albedo,texCoords).rgb);
	gAlbedoSpec.a = 1.0f;
	gARM.rgb = arm;
	
}