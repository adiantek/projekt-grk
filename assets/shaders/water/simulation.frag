#version 300 es

precision highp float;

struct Wave {
    float Q; // stepness
    float A; // Amplitude
    vec2 D; // direction
    float w; // w = sqrt(2.0 * 3.14 * 9.8 / waves[i].l)
    float fi; // fi = 2.0 * waves[i].s / waves[i].l
};

uniform int waveCount;
uniform float time;
uniform float stepness[10];
uniform Wave waves[10];

in vec2 position;

layout(location = 0) out vec3 heightMap;
layout(location = 1) out vec3 normalMap;

void main() {
    vec3 height = vec3(0.0);
    vec3 normal = vec3(0.0, 0.0, 1.0);

    for(int i=0; i<waveCount; ++i) {
        float value = dot(waves[i].D, position) * waves[i].w + waves[i].fi * time;

        height.x += waves[i].Q * waves[i].A * waves[i].D.x * cos(value);
        height.y += waves[i].Q * waves[i].A * waves[i].D.y * cos(value);
        height.z += waves[i].A * sin(value);

        normal.x -= waves[i].D.x * waves[i].w * waves[i].A * cos(value);
        normal.y -= waves[i].D.y * waves[i].w * waves[i].A * cos(value);
        normal.z -= waves[i].Q * waves[i].w * waves[i].A * sin(value);
    }

    heightMap = height;
    normalMap = normalize(normal);
}