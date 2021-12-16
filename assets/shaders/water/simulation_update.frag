#version 300 es

precision highp float;

uniform sampler2D depthMap;
uniform vec2 delta;

in vec2 texturePosition;

void main() {
    vec2 texPos = vec2(texturePosition.x, 1.0 - texturePosition.y);
    vec4 info = texture(depthMap, texPos);

    vec2 dx = vec2(delta.x, 0.0);
    vec2 dy = vec2(0.0, delta.y);

    float average = (
        texture(depthMap, texPos - dx).r +
        texture(depthMap, texPos - dy).r +
        texture(depthMap, texPos + dx).r +
        texture(depthMap, texPos + dy).r
    ) * 0.25;

    /* change the velocity to move toward the average */
    info.g += (average - info.r) * 2.0;

    /* attenuate the velocity a little so waves do not last forever */
    info.g *= 0.995;

    /* move the vertex along the velocity */
    info.r += info.g;

    /* update the normal */
    vec3 ddx = vec3(delta.x, texture(depthMap, vec2(texPos.x + delta.x, texPos.y)).r - info.r, 0.0);
    vec3 ddy = vec3(0.0, texture(depthMap, vec2(texPos.x, texPos.y + delta.y)).r - info.r, delta.y);
    info.ba = normalize(cross(ddy, ddx)).xz;

    gl_FragColor = info;
}