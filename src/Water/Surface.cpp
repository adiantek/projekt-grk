#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Water/Surface.hpp>

namespace water {
Surface::Surface(float size, float y, unsigned int textureSize, unsigned int heightMap, unsigned int normalMap, glm::vec2 offset) {
    this->size = size;
    this->y = y;
    this->heightMap = heightMap;
    this->normalMap = normalMap;
    this->offset = offset;
    this->geometry.initPlane(size, size, textureSize, textureSize);
    this->skybox = resourceLoaderExternal->tex_skybox;
}

Surface::~Surface() {}

void Surface::draw(glm::mat4 viewMatrix) {
    glUseProgram(resourceLoaderExternal->p_water_surface);

    glm::mat4 model = glm::translate(glm::vec3(camera->position.x + this->offset.x, this->y, camera->position.z + this->offset.y)) * this->rotation;
    glm::mat4 transformation = viewMatrix * model;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->heightMap);
    glUniform1i(resourceLoaderExternal->p_water_surface_uni_heightMap, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, this->normalMap);
    glUniform1i(resourceLoaderExternal->p_water_surface_uni_normalMap, 1);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skybox);
    glUniform1i(resourceLoaderExternal->p_water_surface_uni_skybox, 2);

    glUniformMatrix4fv(resourceLoaderExternal->p_water_surface_uni_transformation, 1, GL_FALSE, (float*)&transformation);
    glUniformMatrix4fv(resourceLoaderExternal->p_water_surface_uni_model, 1, GL_FALSE, (float*)&model);
    glUniform3f(resourceLoaderExternal->p_water_surface_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);

    Core::DrawContext(this->geometry);

    glUseProgram(0);
}
}  // namespace water