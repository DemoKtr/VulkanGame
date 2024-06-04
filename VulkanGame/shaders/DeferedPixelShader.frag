#version 450

layout (input_attachment_index = 0, binding = 0) uniform subpassInput inputPosition;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput inputNormal;
layout (input_attachment_index = 2, binding = 2) uniform subpassInput inputAlbedo;
layout (input_attachment_index = 3, binding = 3) uniform subpassInput inputARM;
layout (input_attachment_index = 4, binding = 4) uniform subpassInput inputT;


layout(set = 0,binding = 5) uniform PointLight{
		vec3 position[2];
		vec3 diffuse[2];
		vec3 camPos;

} light;

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
	vec3 camPos = TBN * light.camPos;
	
	vec3 V = normalize(camPos - worldPos);
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metalic);
	vec3 Lo = vec3(0.0);
	//for(int i=0; i<2; ++i) {}
	Lo +=  lightCalc(worldPos,(TBN*vec3(0.0f,2.0f,0.0f)),V,vec3(255.0f,255.0f,255.0f),N,F0,albedo,roughness,metalic);
	
	 vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    outColor = vec4(color, 1.0f);
	
}