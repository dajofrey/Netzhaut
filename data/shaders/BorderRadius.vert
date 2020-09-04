#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform _ubuffer {
    mat4 MVP;
    vec3 color;
    vec4 corners;
    vec2 pixelSize;
} ubuffer;

layout(location = 0) in  vec3 inPosition;
layout(location = 0) out vec3 outColor;
layout(location = 1) out vec2 outUV;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() 
{
    gl_Position = ubuffer.MVP * vec4(inPosition, 1.0);
    float x = (gl_Position.r / 2) + 0.5;
    float y = (gl_Position.g / 2) + 0.5;
    outUV = vec2(x, y);
    outColor = ubuffer.color;
}
