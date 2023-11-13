#version 450

layout(binding = 1) uniform sampler2D samplerColor;
layout(binding = 2) uniform UBO {
    vec4  textColor;
    vec4  outlineColor;
    float outlineWidth;
    float outline;
    float size;
} ubo;

layout(location = 0) in  vec2 inUV;
layout(location = 0) out vec4 outColor;

void main() 
{
    float dist = abs(texture(samplerColor, inUV).r - 1);
    outColor = vec4(0.0);

    float noAlphaUpTo;
    float ignoreFrom;

    if (ubo.size <= 12) {
        noAlphaUpTo = 0.4;
        ignoreFrom = 0.9;
    }
    else if (ubo.size >= 13 && ubo.size <= 17) {
        noAlphaUpTo = 0.3;
        ignoreFrom = 1.0;
    }
    else {
        noAlphaUpTo = 0.2;
        ignoreFrom = 1.0;
    }

    if (dist < ignoreFrom) {
        outColor = vec4(ubo.textColor.rgb, 1.0);
        if (dist > noAlphaUpTo) {
            float a = abs(((dist - noAlphaUpTo) / (ignoreFrom - noAlphaUpTo)) - 1.0);
            outColor = vec4(a * ubo.textColor.rgba);
        }
    }
}
