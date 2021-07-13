#version 330 core
precision mediump float;
out vec4 FragColor;
uniform vec2 resolution;

float sphereSize = 0.6;
vec3 spherePosition = vec3(0.5, 0.5, 0.5);

float dist_func(vec3 position, float size) {
    return length(position) - size;
}

vec3 normal(vec3 pos, float size) {
    float ep = 0.0001;
    return normalize(vec3(
        dist_func(pos, size) - dist_func(vec3(pos.x - ep, pos.y, pos.z), size),
        dist_func(pos, size) - dist_func(vec3(pos.x, pos.y - ep, pos.z), size),
        dist_func(pos, size) - dist_func(vec3(pos.x, pos.y, pos.z - ep), size)
    ));
}

void main(void) {
    vec2 pixelPosition2d = (gl_FragCoord.xy * 2.0 - resolution.xy) / min(resolution.x, resolution.y);
    vec3 cameraPosition = vec3(0.0, 0.0, 10.0);
    float screenZ = 4.0;
    vec3 pixelPosition = vec3(pixelPosition2d, screenZ);
    
    vec3 lightDirection = normalize(vec3(0.0, 0.0, 1.0));
    vec3 rayDirection = normalize(pixelPosition - cameraPosition);
    vec3 color = vec3(0.0);
    
    for (int i = 0; i < 99; i++) {
        vec3 cameraToSphere = cameraPosition-spherePosition;
        float dist = dist_func(cameraToSphere, sphereSize);
        if (dist < 0.0001) {
            vec3 norm = normal(cameraToSphere, sphereSize);
            float diff = dot(norm, lightDirection);
            color = vec3(diff);
            break;
        }
        cameraPosition += rayDirection * dist;
    }
    
    FragColor = vec4(color, 1.0);
}
