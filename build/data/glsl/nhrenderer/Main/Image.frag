#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D samplerColor;

layout(location = 0) in vec2 inUV;
layout(location = 1) in float inGamma;

layout(location = 0) out vec4 outColor;

void main() 
{
    vec4 rawColor = texture(samplerColor, inUV);
    outColor.r = pow(rawColor.r, inGamma);
    outColor.g = pow(rawColor.g, inGamma);
    outColor.b = pow(rawColor.b, inGamma);
    outColor.a = rawColor.a;
}
