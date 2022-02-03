#include <Logger.h>

#include <Camera/Camera.hpp>
#include <Random.hpp>
#include <ResourceLoader.hpp>
#include <Time/Time.hpp>
#include <Water/Simulation.hpp>

namespace water {
Simulation::Simulation(float size, unsigned int textureSize, glm::vec2 offset) {
    this->size = size / 2.0f;
    this->textureSize = textureSize;
    this->offset = offset;
    this->geometry.initPlane(2.0f, 2.0f);
    // Create framebuffer for rendering water simulation
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    // Create textures for maps of height and normal
    glGenTextures(2, this->maps);
    // Init height map texture
    glBindTexture(GL_TEXTURE_2D, this->maps[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureSize, textureSize, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Init normal map texture
    glBindTexture(GL_TEXTURE_2D, this->maps[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureSize, textureSize, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Bind textures to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->maps[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->maps[1], 0);
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);
    // Check for framebuffer errors
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Framebuffer not created");
    }
    // Bind previous framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Simulation::~Simulation() {
    glDeleteTextures(2, this->maps);
    glDeleteFramebuffers(1, &this->framebuffer);
}

void Simulation::update() {
    glUseProgram(resourceLoaderExternal->p_water_simulation);

    glUniform1f(resourceLoaderExternal->p_water_simulation_uni_scale, this->size);
    glUniform1f(resourceLoaderExternal->p_water_simulation_uni_time, (float)timeExternal->lastFrame);
    glUniform2f(resourceLoaderExternal->p_water_simulation_uni_transition, camera->position.x + this->offset.x, camera->position.z + this->offset.y);

    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    glViewport(0, 0, this->textureSize, this->textureSize);

    Core::DrawContext(this->geometry);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    camera->useCameraViewport();
}

void Simulation::generateRandomWaves(long long seed) {
    Random random(seed);
    int waveCount = 10;
    float windAngle = random.nextFloat(0.0f, 100.0f);
    glm::vec2 windDirection = glm::vec2(cosf(windAngle), sinf(windAngle));

    if (windDirection.y < 0.0f)
        windAngle = -windAngle;

    std::string format("waves[i].X");

    glUseProgram(resourceLoaderExternal->p_water_simulation);
    glUniform1i(resourceLoaderExternal->p_water_simulation_uni_waveCount, waveCount);

    for (int i = 0; i < waveCount; ++i) {
        float amplitude = random.nextFloat(0.01f, 0.04f);
        float stepness = random.nextFloat(0.3f, 0.4f);
        float waveAngle = random.nextFloat(windAngle - glm::radians(27.0f), windAngle + glm::radians(27.0f));
        glm::vec2 waveDirection = glm::vec2(cosf(waveAngle), sinf(waveAngle));
        float speed = random.nextFloat(1.5f, 3.5f);
        float lenght = random.nextFloat(50.0f, 100.0f) * amplitude;

        float w = sqrt(2.0f * 3.14159f * 9.8f / lenght);
        float f = 2.0f * speed / lenght;

        format[6] = i + '0';

        format[9] = 'A';
        glUniform1f(glGetUniformLocation(resourceLoaderExternal->p_water_simulation, format.data()), amplitude);
        format[9] = 'Q';
        glUniform1f(glGetUniformLocation(resourceLoaderExternal->p_water_simulation, format.data()), stepness);
        format[9] = 'D';
        glUniform2f(glGetUniformLocation(resourceLoaderExternal->p_water_simulation, format.data()), waveDirection.x, waveDirection.y);
        format[9] = 'w';
        glUniform1f(glGetUniformLocation(resourceLoaderExternal->p_water_simulation, format.data()), w);
        format[9] = 'f';
        glUniform1f(glGetUniformLocation(resourceLoaderExternal->p_water_simulation, format.data()), f);
    }
}

unsigned int Simulation::getHeightMap() {
    return this->maps[0];
}

unsigned int Simulation::getNormalMap() {
    return this->maps[1];
}
}  // namespace water