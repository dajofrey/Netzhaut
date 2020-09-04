float circle(
    in vec2 _st, in float _radius)
{
    vec2 dist = _st-vec2(0.5);
    return 1.0 - 
        smoothstep(_radius-(_radius*0.01), _radius+(_radius*0.01), dot(dist,dist)*4.0);
}

vec4 radius(
    vec2 UV, vec2 dimensions, float radius, vec3 fragColor) 
{
    vec2 coords = UV * dimensions;
    if (length(coords) < radius)
    {
        vec2 relCoords = vec2(coords.x/radius, coords.y/radius);
        float inCircle = circle(relCoords,1.0);
        vec3 color = vec3(inCircle * fragColor.r, inCircle * fragColor.g, inCircle * fragColor.b);
        vec4 outColor = vec4(color, 1.0);
        if (coords.x >= radius / 2 || coords.y >= radius / 2) {outColor = vec4(fragColor, 1.0);}
        return outColor;
    }
    else if (length(coords - vec2(dimensions.x, 0)) < radius)
    {
        vec2 relCoords = vec2(abs(coords.x - dimensions.x) / radius, coords.y / radius);
        float inCircle = circle(relCoords,1.0);
        vec3 color = vec3(inCircle * fragColor.r, inCircle * fragColor.g, inCircle * fragColor.b);
        vec4 outColor = vec4(color, 1.0);
        if (abs(coords.x - dimensions.x) >= radius / 2 || coords.y >= radius / 2) {
            outColor = vec4(fragColor, 1.0);
        }
        return outColor;
    }
    // if (length(coords - vec2(0, dimensions.y)) < radius)
    // {
    //     vec2 relCoords = vec2(coords.x / radius, abs(coords.y - dimensions.y) / radius);
    //     float inCircle = circle(relCoords,1.0);
    //     vec3 color = vec3(inCircle * fragColor.r, inCircle * fragColor.g, inCircle * fragColor.b);
    //     outColor = vec4(color, 1.0);
    //     if (coords.x >= radius / 2 || abs(coords.y - dimensions.y) >= radius / 2)
    //         outColor = vec4(fragColor, 1.0);
    // }
    // else if (length(coords - vec2(dimensions.x, 0)) < radius)
    // {
    //     vec2 relCoords = vec2(abs(coords.x - dimensions.x) / radius, coords.y / radius);
    //     float inCircle = circle(relCoords,1.0);
    //     vec3 color = vec3(inCircle * fragColor.r, inCircle * fragColor.g, inCircle * fragColor.b);
    //     outColor = vec4(color, 1.0);
    //     if (abs(coords.x - dimensions.x) >= radius / 2 || coords.y >= radius / 2)
    //         outColor = vec4(fragColor, 1.0);
    // }
    
    return vec4(fragColor, 1.0);
}

