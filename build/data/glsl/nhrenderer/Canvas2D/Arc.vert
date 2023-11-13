#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform _ubuffer {
    mat4 MVP;    
    float colorR;
    float colorG;
    float colorB;
    float backgroundColorR;
    float backgroundColorG;
    float backgroundColorB;
    float width;
    float height;
    float x;
    float y;
    float radius;
    float startAngle;
    float endAngle;
} ubuffer;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outBackgroundColor;
layout(location = 2) out vec2 outCanvas;
layout(location = 3) out vec2 outCenter;
layout(location = 4) out float outRadius;
layout(location = 5) out float outStartAngle;
layout(location = 6) out float outEndAngle;
layout(location = 7) out vec2 outUV;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() 
{
    outColor = vec3(ubuffer.colorR, ubuffer.colorG, ubuffer.colorB);
    outBackgroundColor = vec3(ubuffer.backgroundColorR, ubuffer.backgroundColorG, ubuffer.backgroundColorB);
    outCanvas     = vec2(ubuffer.width, ubuffer.height);
    outCenter     = vec2(ubuffer.x, ubuffer.y);
    outRadius     = ubuffer.radius;
    outStartAngle = ubuffer.startAngle;
    outEndAngle   = ubuffer.endAngle;

    outUV = inUV;

    gl_Position = ubuffer.MVP * vec4(inPosition, 1.0); 
}
