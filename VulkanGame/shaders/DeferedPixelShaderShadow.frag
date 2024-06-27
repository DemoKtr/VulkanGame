#version 450

layout (input_attachment_index = 0, binding = 0) uniform subpassInput inputPosition;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput inputNormal;
layout (input_attachment_index = 2, binding = 2) uniform subpassInput inputAlbedo;
layout (input_attachment_index = 3, binding = 3) uniform subpassInput inputARM;
layout (input_attachment_index = 4, binding = 4) uniform subpassInput inputT;
layout (input_attachment_index = 5, binding = 8) uniform subpassInput inputWorldPos;

layout(set=0,binding=7) uniform sampler2DArray depthMap;

struct PointLight{
		vec4 position;
		vec4 diffuse;
		mat4 mvp[6];
} ;

layout(std140, set = 0, binding = 5)readonly  buffer PointLights {
    PointLight lights[];
} light;

layout(set = 0,binding = 6) uniform CAMPOS {
	vec4 camPos;
} cameraPosition;

#define SHADOW_FACTOR 0.25

const float PI = 3.14159265359;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 lightCalc(vec3 worldPos, vec3 lightPos, vec3 V,vec3 diffuse,vec3 N,vec3 F0,vec3 albedo,float roughness,float metallic){
	vec3 L = normalize(lightPos - worldPos);
	vec3 H = normalize(V + L);
	float distance = length(lightPos - worldPos);
	float attenuation = 1.0 / (distance * distance);
    vec3 radiance = diffuse * attenuation;
	
	// Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
    vec3 numerator    = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
	
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;
	float NdotL = max(dot(N, L), 0.0);  
	return (kD * albedo / PI + specular) * radiance * NdotL;
	
}


float textureProj(vec4 P, float layer, vec2 offset)
{
	float shadow = 1.0;
	vec4 shadowCoord = P / P.w;
	shadowCoord.st = shadowCoord.st * 0.5 + 0.5;
	
	if (shadowCoord.z > -1.0 && shadowCoord.z < 1.0) 
	{
		float dist = texture(depthMap, vec3(shadowCoord.st + offset, layer)).r;
		if (shadowCoord.w > 0.0 && dist < shadowCoord.z) 
		{
			shadow = SHADOW_FACTOR;
		}
	}
	return shadow;
}

float filterPCF(vec4 sc, float layer)
{
	ivec2 texDim = textureSize(depthMap, 0).xy;
	float scale = 1.5;
	float dx = scale * 1.0 / float(texDim.x);
	float dy = scale * 1.0 / float(texDim.y);

	float shadowFactor = 0.0;
	int count = 0;
	int range = 3;
	
	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			shadowFactor += textureProj(sc, layer, vec2(dx*x, dy*y));
			count++;
		}
	
	}
	return shadowFactor / count;
}

vec3 shadow(vec3 fragcolor, vec3 fragpos) {
	
	
	 
	for(int i = 0; i < 2; ++i)
	{
	for(int face=0;face<6;++face){
		vec4 shadowClip	= light.lights[i].mvp[face] * vec4(fragpos, 1.0);
		float shadowFactor = 0.0f;
		shadowFactor = filterPCF(shadowClip, i*6 + face);
		fragcolor *= shadowFactor;
	}
	
	}
	
return fragcolor;
}

void main() {
	
	vec3 worldPos = subpassLoad(inputPosition).rgb;
	vec3 N = normalize(subpassLoad(inputNormal).rgb);
	vec3 albedo = (subpassLoad(inputAlbedo).rgb);
	float ao = (subpassLoad(inputARM).r);
	float roughness = (subpassLoad(inputARM).g);
	float metalic = (subpassLoad(inputARM).b);
	vec3 T = (subpassLoad(inputT).rgb);
	vec3 No = vec3((subpassLoad(inputNormal).a),(subpassLoad(inputARM).a),(subpassLoad(inputT).a));
	T = normalize(T - dot(T, No) * No);
	vec3 B = cross(No, T);
	mat3 TBN = transpose(mat3(T,B,No));
	vec3 camPos = TBN * vec3(cameraPosition.camPos.xyz);
	
	vec3 V = normalize(camPos - worldPos);
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metalic);
	vec3 Lo = vec3(0.0);
	 
	 for(int i=0;i<2;++i)Lo +=  lightCalc(worldPos,(TBN * vec3(light.lights[i].position)),V,vec3(light.lights[i].diffuse),N,F0,albedo,roughness,metalic);
	 vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

	vec3 wp = (subpassLoad(inputWorldPos).rgb);
	vec3 world = inverse(TBN)*(worldPos);
	//color = shadow(vec3(color),world);


    outColor = vec4(color,subpassLoad(inputPosition).a);
	
}