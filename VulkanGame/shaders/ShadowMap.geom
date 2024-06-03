#version 450

#define POINT_LIGHT_COUNT 2

layout (triangles, invocations = POINT_LIGHT_COUNT) in;
layout (triangle_strip, max_vertices = 18) out;

layout (binding = 0) uniform UBO 
{
	mat4 shadowMatrices[POINT_LIGHT_COUNT][6]; // Macierze dla sześciu twarzy cubemapy dla każdego światła
	vec4 instancePos[POINT_LIGHT_COUNT]; // Pozycje instancji dla każdego światła
} ubo;

layout (location = 0) in int inInstanceIndex[];



void main()
{
	int lightIndex = gl_InvocationID; // Aktualne światło punktowe
	vec4 instancedPos = ubo.instancePos[lightIndex]; // Pozycja instancji dla aktualnego światła

	// Iteracja przez wszystkie sześć twarzy cubemapy dla aktualnego światła
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = lightIndex * 6 + face; // wbudowana zmienna, która określa, do której twarzy renderujemy

		// Iteracja przez wierzchołki trójkąta
		for (int i = 0; i < gl_in.length(); ++i)
		{
			vec4 FragPos = gl_in[i].gl_Position + instancedPos;
			gl_Position = ubo.shadowMatrices[lightIndex][face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}