#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec3 inBackgroundColor;
layout(location = 2) in vec2 inCanvas;
layout(location = 3) in vec2 inCenter;
layout(location = 4) in float inRadius;
layout(location = 5) in float inStartAngle;
layout(location = 6) in float inEndAngle;
layout(location = 7) in vec2 inUV;

layout(location = 0) out vec4 outColor;

void main() 
{
    vec2 fragCoords = vec2(inUV.x * inCanvas.x, inUV.y * inCanvas.y);
    
    vec2 normalizedCenter = vec2(inCenter.x/inCanvas.x, inCenter.y/inCanvas.y);
    float currentRadiusInPixel = sqrt(pow(fragCoords.x - inCenter.x, 2.0) + pow(fragCoords.y - inCenter.y, 2.0));                                              
    float radians = 3.1415 - inEndAngle;
    float lineWidthInPixel = 3.0;

    outColor = vec4(inBackgroundColor, 1.0);

    if (atan(normalizedCenter.y - inUV.y, normalizedCenter.x - inUV.x) > radians) {
        if (currentRadiusInPixel < inRadius && currentRadiusInPixel >= inRadius - lineWidthInPixel) {
            outColor = vec4(inColor, 1.0);
        } 
    } 
}
