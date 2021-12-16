#include<Water.hpp>

Water::Water(float x, float y, float z, float width, float height, int simulationWidth, int simulationHeight, ResourceLoader* loader) {
    glGenFramebuffers(1, &this->frameBuffer);
    glGenTextures(2, this->textures);

    glBindTexture(GL_TEXTURE_2D, this->textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, simulationWidth, simulationHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // TODO: load next texure instead of wrap?
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, this->textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, simulationWidth, simulationHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // TODO: load next texure instead of wrap?
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    this->geometry.initPlane(width, height, simulationWidth, simulationHeight);
    this->dummyGeometry.initPlane(2.0f, 2.0f);

    this->translation = glm::vec3(x, y, z);

    this->frameBufferWidth = simulationWidth;
    this->frameBufferHeight = simulationHeight;

    this->loader = loader;

    glUniform2f(loader->p_water_simulation_update_uni_delta, 1.0f / 216.0f, 1.0f / 216.0f);
}

Water::~Water() {

}

void Water::stepSimulation() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->textures[1], 0);

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    glViewport(0, 0, this->frameBufferWidth, this->frameBufferHeight);

    glUniform1i(loader->p_water_simulation_update_uni_depthMap, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textures[0]);

    Core::DrawContext(this->dummyGeometry);

    unsigned int help = this->textures[0];
    this->textures[0] = this->textures[1];
    this->textures[1] = help;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::addDrop(float x, float y, float radius, float strength) {
    glUniform2f(loader->p_water_simulation_drop_uni_center, x, y);
    glUniform1f(loader->p_water_simulation_drop_uni_radius, radius);
    glUniform1f(loader->p_water_simulation_drop_uni_strength, strength);

    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->textures[1], 0);

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    glViewport(0, 0, this->frameBufferWidth, this->frameBufferHeight);

    glUniform1i(loader->p_water_simulation_drop_uni_depthMap, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textures[0]);

    Core::DrawContext(this->dummyGeometry);

    unsigned int help = this->textures[0];
    this->textures[0] = this->textures[1];
    this->textures[1] = help;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::renderSimulation() {

}