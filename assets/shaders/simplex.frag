#version 300 es
precision highp float;
precision highp int;

in vec2 fragPos;
out vec4 FragColor;

uniform vec2 translation[4];
uniform float scale[4];
uniform float weight[4];
uniform sampler2D p;

const float grads[24] = float[24](
    1.0, 1.0,
    -1.0, 1.0,
    1.0, -1.0,
    -1.0, -1.0,
    1.0, 0.0,
    -1.0, 0.0,
    1.0, 0.0,
    -1.0, 0.0,
    0.0, 1.0,
    0.0, -1.0,
    0.0, 1.0,
    0.0, -1.0);

// https://en.wikipedia.org/wiki/Simplex_noise#Algorithm_detail
// n = 2

const float SQRT_3 = sqrt(3.0);
const float F_2 = (SQRT_3 - 1.0) / 2.0;
const float G_2 = (3.0 - SQRT_3) / 6.0; // alternate form to (1-1/sqrt(3))/2 from WolframAlpha

const vec2 F_22 = vec2(F_2, F_2);
const vec2 G_22 = vec2(G_2, G_2);

int getPermutValue(int permutIndex, float layer) {
    return int(texture(p, vec2(float(permutIndex) / 256.0, layer / 4.0)).r * 256.0);
}

float processGrad(int gradIndex, vec2 xy) {
    return
        grads[gradIndex * 2 + 0] * xy.x +
        grads[gradIndex * 2 + 1] * xy.y;
}

float getContrib(int gradIndex, vec2 xy, float offset) {
    float d1 = offset - dot(xy.xy, xy.xy);
    float d0;
    if (d1 < 0.0) {
        d0 = 0.0;
    } else {
        d1 = d1 * d1;
        d0 = d1 * d1 * processGrad(gradIndex, xy);
    }
    return d0;
}

// https://weber.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf page 11
float getValue(vec2 v, float layer) {
    // Find unit grid cell containing point
    vec2 ij = floor(v + dot(v, F_22));
    float xy = dot(ij, G_22);
    vec2 xy0 = v - ij + xy; // The x,y distances from the cell origin

    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    vec2 kl = (xy0.x > xy0.y) ? // Offsets for second (middle) corner of simplex in (i,j) coords
        vec2(1.0, 0.0) // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        : vec2(0.0, 1.0); // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    vec2 xy1 = xy0 - kl + G_2; // Offsets for middle corner in (x,y) unskewed coords
    vec2 xy2 = xy0 - 1.0 + 2.0 * G_2; // Offsets for last corner in (x,y) unskewed coords
    
    // Wrap the integer cells at 255 (smaller integer period can be introduced here)
    ivec2 ij2 = ivec2(ij) & 255;
    
    // Work out the hashed gradient indices of the three simplex corners
    int gi0 = getPermutValue(ij2.x + getPermutValue(ij2.y, layer), layer) % 12;
    int gi1 = getPermutValue(ij2.x + int(kl.x) + getPermutValue(ij2.y + int(kl.y), layer), layer) % 12;
    int gi2 = getPermutValue(ij2.x + 1 + getPermutValue(ij2.y + 1, layer), layer) % 12;

    float d10 = getContrib(gi0, xy0, 0.5); // Calculate the contribution from the first corner
    float d11 = getContrib(gi1, xy1, 0.5); // Calculate the contribution from the second corner
    float d12 = getContrib(gi2, xy2, 0.5); // Calculate the contribution from the third corner
    
    return 70.0 * (d10 + d11 + d12);
}

void main()
{
    float noise =
        getValue((fragPos + translation[0]) * scale[0], 0.0) * weight[0] +
        getValue((fragPos + translation[1]) * scale[1], 1.0) * weight[1] +
        getValue((fragPos + translation[2]) * scale[2], 2.0) * weight[2] +
        getValue((fragPos + translation[3]) * scale[3], 3.0) * weight[3];
    FragColor.r = noise;
}
