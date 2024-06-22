#version 450

layout(location = 3) out vec4 gAlbedoSpec;


layout (location = 0) in vec2 inUV;



layout(set=0,binding=0) uniform sampler2D ssaoInput;




void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoInput, inUV + offset).r;
        }
    }
    gAlbedoSpec.a = result / (4.0 * 4.0);
}  