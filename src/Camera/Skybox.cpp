#include <opengl.h>

#include <Camera/Camera.hpp>
#include <Camera/Skybox.hpp>
#include <ResourceLoader.hpp>
#include <glm/glm.hpp>
#include <vertex/VertexBuffer.hpp>

using namespace cam;
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
    this->vbo = skyboxVBO.uploadVBO();
    skyboxVBO.configureVAO(0, 3, GL_FLOAT, GL_FALSE, skyboxVBO.getFormat()->pos);
}

Skybox::~Skybox(){
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->VAO);
}

void Skybox::update() {
}

void Skybox::draw(glm::mat4 mat) {
    glm::mat4 view = glm::translate(camera->getPosition());

    glDisable(GL_DEPTH_TEST);
    glUseProgram(resourceLoaderExternal->p_skybox_shader);
    glUniformMatrix4fv(resourceLoaderExternal->p_skybox_shader_uni_projection, 1, GL_FALSE, glm::value_ptr(mat * view));
    // skybox cube
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, resourceLoaderExternal->tex_skybox);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
    glEnable(GL_DEPTH_TEST);
}