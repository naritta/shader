#version 330 core
precision mediump float;
uniform float t;
out vec4 FragColor;
uniform vec2 resolution;

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct Sphere{
    float radius;
    vec3  position;
    vec3  color;
};

vec3 lightDirection = normalize(vec3(0.0, 0.0, 1.0));

vec3 intersectSphere(Ray R, Sphere S){
    vec3  a = R.origin - S.position;
    float b = dot(a, R.direction);
    float c = dot(a, a) - (S.radius * S.radius);
    float d = b * b - c;
    if(d > 0.0){
        float t = -b - sqrt(d);
        vec3 hitPoint = R.origin + R.direction * t;
        vec3 normal = normalize(hitPoint - S.position);
        float diff = dot(normal, lightDirection);
        vec3 color = vec3(diff);
        return color;
    }
    return vec3(0.0);
}

void main(void){
    // fragment position
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    
    // ray init
    Ray ray;
    ray.origin = vec3(0.0, 0.0, 5.0);
    ray.direction = normalize(vec3(p.x, p.y, -1.0));
    
    // sphere init
    Sphere sphere;
    sphere.radius = 1.0;
    sphere.position = vec3(0.0);
    sphere.color = vec3(1.0);
    
    // hit check
    vec3 destColor = vec3(0.0);
    destColor = intersectSphere(ray, sphere);
    
    FragColor = vec4(destColor, 1.0);
}
