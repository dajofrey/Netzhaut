#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform _ubuffer {
    mat4 MVP;
    float gamma;
    float xRepeat;
    float yRepeat;
} ubuffer;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 outUV;
layout(location = 1) out float outGamma;
layout(location = 2) out float xRepeat;
layout(location = 3) out float yRepeat;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() 
{
    gl_Position = ubuffer.MVP * vec4(inPosition, 1.0); 
    outUV = inUV;
    outGamma = ubuffer.gamma;
    xRepeat = ubuffer.xRepeat;
    yRepeat = ubuffer.yRepeat;
}
