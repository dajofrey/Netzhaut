#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : require

#include "Radius.glsl"

layout(location = 0) in  vec4 inColor;
layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = inColor;
}
