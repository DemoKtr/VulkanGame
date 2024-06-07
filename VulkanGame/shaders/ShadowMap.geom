#version 450

#define POINT_LIGHT_COUNT 2

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

struct PointLight{
		vec4 position;
		vec4 diffuse;
		mat4 mvp[6];
} ;

layout(std140, set = 0, binding = 1)readonly  buffer PointLights {
    PointLight lights[];
} light;

layout(location = 0) in int outInstanceIndex[];
layout(location = 1) in mat4 modelMatrix[];
layout(location = 5) in mat3 tbnMatrix[];




void main()
{
	int lightIndex = gl_InvocationID; // Aktualne światło punktowe
	
	for(int l=0;l<2;++l){
	
	// Iteracja przez wszystkie sześć twarzy cubemapy dla aktualnego światła
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = l* 6 + face; // wbudowana zmienna, która określa, do której twarzy renderujemy
		for (int i = 0; i < 3; ++i)
		{
			vec4 FragPos = gl_in[i].gl_Position;
			FragPos =  modelMatrix[0] * FragPos;
			//FragPos =  vec4(tbnMatrix[0] * vec3(FragPos).xyz,1.0f);
			gl_Position = light.lights[l].mvp[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
	}
}