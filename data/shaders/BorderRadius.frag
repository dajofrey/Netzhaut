#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in  vec3 inColor;
layout(location = 1) in  vec2 inUV;
layout(location = 0) out vec4 outColor;

void main(void) 
{
    outColor = vec4(inColor, 1.0f);
}
