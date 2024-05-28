#version 450
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;


layout(set = 0,binding = 0) uniform UBO {
	mat4 shadowMatrices[6];
} shadowMat;

layout (location = 0) out vec4 FragPos; // FragPos from GS (output per emitvertex)


void main(void) {
	for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMat.shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}