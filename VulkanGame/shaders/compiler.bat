glslangValidator -V VertexShader.vert -o vert.spv
glslangValidator -V GeometryVertexShader.vert -o geoVert.spv

glslangValidator -V PixelShader.frag -o frag.spv
glslangValidator -V GeometryPixelShader.frag -o geoFrag.spv

pause