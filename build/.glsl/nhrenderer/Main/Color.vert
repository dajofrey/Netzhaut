#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform _ubuffer {
    mat4 MVP;
    vec4 color;
} ubuffer;

layout(location = 0) in  vec3 inPosition;
layout(location = 0) out vec4 outColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() 
{
    gl_Position = ubuffer.MVP * vec4(inPosition, 1.0);
    outColor = ubuffer.color;
}
