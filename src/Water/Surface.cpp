#include<Water/Surface.hpp>
#include<Camera/Camera.hpp>

namespace water {
    Surface::Surface(float x, float y, float z, float width, float height, int simulationWidth, int simulationHeight, ResourceLoader* loader)
    : simulation(simulationWidth, simulationHeight, width, loader), 
      caustics(width, simulationWidth * 3, y, this->simulation.maps[0], this->simulation.maps[1]) {
        this->geometry.initPlane(width, height, simulationWidth, simulationHeight);
        this->translation = glm::vec3(x, y, z);
        this->skybox = loader->txt_skybox;
        this->program = loader->p_water_surface;
        this->uniformCamera = loader->p_water_surface_uni_cameraPosition;
        this->uniformModel = loader->p_water_surface_uni_model;
        this->uniformTransformation = loader->p_water_surface_uni_transformation;

        glUseProgram(this->program);
        glUniform1i(loader->p_water_surface_uni_heightMap, 0);
        glUniform1i(loader->p_water_surface_uni_normalMap, 1);
        glUniform1i(loader->p_water_surface_uni_skybox, 2);
        glUseProgram(0);
    }
    
    Surface::~Surface() {}

    void Surface::draw(glm::mat4 viewMatrix, glm::vec3 cameraPos) {
        glm::vec3 transition = glm::vec3(camera->position.x, this->translation.y, camera->position.z);

        this->simulation.simulate(/*this->translation*/transition);

        glUseProgram(this->program);

        glm::mat4 model = glm::translate(/*this->translation*/transition) * this->rotation;
        glm::mat4 transformation = viewMatrix * model;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->simulation.maps[0]);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, this->simulation.maps[1]);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->skybox);
        glUniformMatrix4fv(this->uniformTransformation, 1, GL_FALSE, (float*) &transformation);
        glUniformMatrix4fv(this->uniformModel, 1, GL_FALSE, (float*) &model);
        glUniform3f(this->uniformCamera, cameraPos.x, cameraPos.y, cameraPos.z);

        Core::DrawContext(this->geometry);

        glUseProgram(0);
    }
}