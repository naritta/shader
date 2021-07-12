#version 330 core
precision mediump float;
out vec4 FragColor;
uniform vec2 resolution;

float sphereSize = 0.6;

float sphereDistanceFunction(vec3 position, float size) {
    return length(position) - size;
}

void main( void ) {
    vec2 position = (gl_FragCoord.xy * 2.0 - resolution.xy) / min(resolution.x, resolution.y);
    vec3 cameraPosition = vec3(0.0, 0.0, 10.0);
    float screenZ = 4.0;
    
    vec3 rayDirection = normalize(vec3(position, screenZ) - cameraPosition);
    vec3 color = vec3(0.0);
    
    for (int i = 0; i < 99; i++) {
        vec3 rayPosition = cameraPosition + rayDirection;
        float dist = sphereDistanceFunction(rayPosition, sphereSize);
        
        if (dist < 0.0001) {
            color = vec3(1.0);
            break;
        }
        
        cameraPosition += rayDirection * dist;
    }
    
    FragColor = vec4(color, 1.0);
}
