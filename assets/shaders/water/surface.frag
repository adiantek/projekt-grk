#version 300 es

precision highp float;

uniform samplerCube skybox;
uniform vec3 cameraPosition;

in vec3 normal;
in vec3 viewDirection;
in vec3 fragmentPosition;


out vec4 fragmentColor;


void main() {
    vec3 reflectVector = normalize(reflect(viewDirection, normal));

    float thetaI = acos(dot(viewDirection, normal));
    float thetaR = acos(dot(reflectVector, normal));
    float thetaT = asin(0.75 * sin(thetaI));

    float reflectivity = 0.0;
    if(abs(thetaI) >= 0.000001) {
        float t1 = sin(thetaT - thetaI);
        float t2 = sin(thetaT + thetaI);
        float t3 = tan(thetaT - thetaI);
        float t4 = tan(thetaT + thetaI);
        reflectivity = 0.5 * (t1 * t1 / (t2 * t2) + t3 * t3 / (t4 * t4));
        if (reflectivity > 1.0)
            reflectivity = 1.0;
    }

    vec4 reflection = texture(skybox, reflectVector);

    vec4 transmission = vec4(0.0778151049, 0.1539574715, 0.1774450798, 1.0);

    fragmentColor = reflectivity * reflection + (1.0 - reflectivity) * transmission;

    float dist = length(cameraPosition - fragmentPosition);
    vec4 fogColor = texture(skybox, vec3(-viewDirection.x, 0.0, -viewDirection.z));

    fragmentColor = mix(fragmentColor, fogColor, 1.0 - exp(-0.004 * dist));
}