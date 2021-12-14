#include <SimplexNoiseGenerator.hpp>
#include <Render_Utils.h>

SimplexNoiseGenerator::SimplexNoiseGenerator(Random *r) {
    this->x = r->nextDouble() * 256.0;
    this->y = r->nextDouble() * 256.0;
    this->z = r->nextDouble() * 256.0;
    for (int i = 0; i < 256; i++) {
        this->permutationTable[i] = i;
    }
    for (int i = 0; i < 256; i++) {
        int j = r->nextInt(256 - i);
        int k = this->permutationTable[i];
        this->permutationTable[i] = this->permutationTable[j + i];
        this->permutationTable[j + i] = k;
    }
    // printf("\n");
    // for (int i = 0; i < 256; i++) {
    //     printf("%d, ", this->permutationTable[i]);
    // }
    // printf("\n");

}

void SimplexNoiseGenerator::draw(ResourceLoader *res) {
    glUseProgram(res->p_simplex);
    glUniform1iv(res->p_simplex_uni_p, 256, this->permutationTable);
    glUniform1f(res->p_simplex_uni_scale, 10.0F);
    glUniform1f(res->p_simplex_uni_alpha, 1.0F);
    
    float vertexArray[16] = {
            -1, -1, 0, 1,
            1, -1, 0, 1,
            -1, 1, 0, 1,
            1, 1, 0, 1
    };
    int indexArray[6] = {0, 1, 2, 1, 2, 3};
    Core::DrawVertexArrayIndexed(vertexArray, indexArray, 6, 4);
}