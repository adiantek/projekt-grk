#include <opengl.h>

#include <Camera/Camera.hpp>
#include <Camera/Skybox.hpp>
#include <ResourceLoader.hpp>
#include <glm/glm.hpp>
#include <vertex/VertexBuffer.hpp>

using namespace vertex;

Skybox::Skybox() {
    VertexBuffer skyboxVBO(&POS, 14);
    (&skyboxVBO)
        // kostka Q3 ma ścieżkę Hamiltona - z zajęć z matematyki dyskretnej
        ->pos(-1.f, 1.f, 1.f)->end()
        ->pos(1.f, 1.f, 1.f)->end()
        ->pos(-1.f, -1.f, 1.f)->end()
        ->pos(1.f, -1.f, 1.f)->end()
        ->pos(1.f, -1.f, -1.f)->end()
        ->pos(1.f, 1.f, 1.f)->end()
        ->pos(1.f, 1.f, -1.f)->end()
        ->pos(-1.f, 1.f, 1.f)->end()
        ->pos(-1.f, 1.f, -1.f)->end()
        ->pos(-1.f, -1.f, 1.f)->end()
        ->pos(-1.f, -1.f, -1.f)->end()
        ->pos(1.f, -1.f, -1.f)->end()
        ->pos(-1.f, 1.f, -1.f)->end()
        ->pos(1.f, 1.f, -1.f)->end();
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    skyboxVBO.uploadVBO();
    skyboxVBO.configureVAO(0, 3, GL_FLOAT, GL_FALSE, skyboxVBO.getFormat()->color);
    cubemapTexture = resourceLoaderExternal->txt_skybox;
};

Skybox::~Skybox(){};

void Skybox::draw() {
    glm::mat4 view = glm::translate(camera->getPosition());
    glm::mat4 projection = camera->getTransformationMatrix();

    glDisable(GL_DEPTH_TEST);
    glUseProgram(resourceLoaderExternal->p_skybox_shader);
    glUniformMatrix4fv(resourceLoaderExternal->p_skybox_shader_uni_projection, 1, GL_FALSE, glm::value_ptr(projection * view));
    // skybox cube
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
    glEnable(GL_DEPTH_TEST);
}