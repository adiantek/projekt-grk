#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Water/Surface.hpp>

namespace water {
Surface::Surface(float size, float y, unsigned int textureSize, glm::vec2 offset) : simulation(size, textureSize, offset) {
    this->size = size;
    this->y = y;
    this->offset = offset;
    this->geometry.initPlane(size, size, textureSize, textureSize);
    this->skybox = resourceLoaderExternal->tex_skybox;
    this->lastCameraPosition = glm::vec2(camera->position.x, camera->position.z);
}

Surface::~Surface() {}

void Surface::draw(glm::mat4 viewMatrix) {
    glEnable(GL_BLEND);
	glBlendColor(1.0f, 1.0f, 1.0f, 0.9f);
	glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

    glUseProgram(resourceLoaderExternal->p_water_surface);

    glm::mat4 model = glm::translate(glm::vec3(this->lastCameraPosition.x + this->offset.x, this->y, this->lastCameraPosition.y + this->offset.y)) * this->rotation;
    glm::mat4 transformation = viewMatrix * model;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->simulation.getHeightMap());
    glUniform1i(resourceLoaderExternal->p_water_surface_uni_heightMap, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, this->simulation.getNormalMap());
    glUniform1i(resourceLoaderExternal->p_water_surface_uni_normalMap, 1);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skybox);
    glUniform1i(resourceLoaderExternal->p_water_surface_uni_skybox, 2);

    glUniformMatrix4fv(resourceLoaderExternal->p_water_surface_uni_transformation, 1, GL_FALSE, (float*)&transformation);
    glUniformMatrix4fv(resourceLoaderExternal->p_water_surface_uni_model, 1, GL_FALSE, (float*)&model);
    glUniform3f(resourceLoaderExternal->p_water_surface_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);

    Core::DrawContext(this->geometry);

    glDisable(GL_BLEND);

    glUseProgram(0);
}

void Surface::update() {
    this->lastCameraPosition = glm::vec2(camera->position.x, camera->position.z);
    this->simulation.update();
}
}  // namespace water