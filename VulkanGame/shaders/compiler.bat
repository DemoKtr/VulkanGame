glslangValidator -V VertexShader.vert -o vert.spv
glslangValidator -V GeometryVertexShader.vert -o geoVert.spv
glslangValidator -V DeferedVertexShader.vert -o deferedVert.spv
glslangValidator -V ShadowMap.vert -o ShadowMapVert.spv

glslangValidator -V ShadowMap.geom -o ShadowMapGeo.spv


glslangValidator -V PixelShader.frag -o frag.spv
glslangValidator -V GeometryPixelShader.frag -o geoFrag.spv
glslangValidator -V DeferedPixelShader.frag -o deferedFrag.spv
glslangValidator -V ShadowMap.frag -o ShadowMapFrag.spv

pause