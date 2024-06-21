#version 450


layout(location = 0) out vec2 outUV;
layout(location = 1) out vec3 forwards;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	// Obliczanie UV na podstawie indeksu wierzchołka
	outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	// Konwersja UV do przestrzeni ekranu [-1, 1]
	gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
	
	
}