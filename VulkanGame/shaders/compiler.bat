glslangValidator -V VertexShader.vert -o vert.spv
glslangValidator -V GeometryVertexShader.vert -o geoVert.spv
glslangValidator -V GeometryAnimatedVertexShader.vert -o geoAnimVert.spv
glslangValidator -V particleVertexShader.vert -o particleVert.spv
glslangValidator -V DeferedVertexShader.vert -o deferedVert.spv
glslangValidator -V ShadowMap.vert -o ShadowMapVert.spv
glslangValidator -V skyBoxVert.vert -o skyBoxVert.spv
glslangValidator -V postProcessCombinedImageVert.vert -o postProcessCombinedImageVert.spv
glslangValidator -V ssaoVert.vert -o ssaoVert.spv

glslangValidator -V ShadowMap.geom -o ShadowMapGeo.spv

glslangValidator -V particleComputeShader.comp -o particleCompute.spv

glslangValidator -V PixelShader.frag -o frag.spv
glslangValidator -V GeometryPixelShader.frag -o geoFrag.spv
glslangValidator -V GeometryAnimatedPixelShader.frag -o geoAnimFrag.spv
glslangValidator -V particleFragmentShader.frag -o particleFrag.spv
glslangValidator -V DeferedPixelShader.frag -o deferedFrag.spv
glslangValidator -V DeferedPixelShaderShadow.frag -o deferedFragShadow.spv
glslangValidator -V ShadowMap.frag -o ShadowMapFrag.spv
glslangValidator -V skyBoxFrag.frag -o skyBoxFrag.spv
glslangValidator -V postProcessCombinedImageFrag.frag -o postProcessCombinedImageFrag.spv
glslangValidator -V ssaoFrag.frag -o ssaoFrag.spv
glslangValidator -V ssaoBlurFrag.frag -o ssaoBlurFrag.spv
glslangValidator -V downScale.frag -o downScaleFrag.spv
glslangValidator -V upScale.frag -o upScaleFrag.spv
glslangValidator -V finalFrag.frag -o finalFrag.spv

pause