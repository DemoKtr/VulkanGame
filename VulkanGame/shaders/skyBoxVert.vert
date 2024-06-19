#version 450

layout(set = 0, binding = 0) uniform CameraVectors {
	vec4 forwards;
	vec4 right;
	vec4 up;
} cameraData;

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
	// Obliczanie kierunku "forwards"
	forwards = normalize(cameraData.forwards.xyz + (outUV.x * 2.0f - 1.0f) * cameraData.right.xyz - (outUV.y * 2.0f - 1.0f) * cameraData.up.xyz);
	//gl_FragDepth = 1.0f;
}