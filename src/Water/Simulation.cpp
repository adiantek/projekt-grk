#include<Water/Simulation.hpp>
#include<Random.hpp>
#include <Logger.h>

static inline float randf(float min, float max, int precision = 1000)
{
    if (min > max) std::swap(min, max);
    float delta = max - min;
    auto i = int(delta * precision);
    return ((float)(rand() % i) / (float)precision) + min;
}

namespace Water {
    Simulation::Simulation(unsigned int width, unsigned int height, float scale, ResourceLoader *loader) {
        this->width = width;
        this->height = height;
        this->scale = scale / 2.0f;
        this->geometry.initPlane(2.0f, 2.0f);

        this->program = loader->p_water_simulation;
        this->uniformTransition = loader->p_water_simulation_uni_transition;
        this->uniformScale = loader->p_water_simulation_uni_scale;
        this->uniformTime = loader->p_water_simulation_uni_time;
        this->uniformWaveCount = loader->p_water_simulation_uni_waveCount;
        // Create framebuffer for rendering water simulation
        glGenFramebuffers(1, &this->framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        // Create textures for maps of height and normal
        glGenTextures(2, this->maps);
        // Init height map texture
        glBindTexture(GL_TEXTURE_2D, this->maps[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Init normal map texture
        glBindTexture(GL_TEXTURE_2D, this->maps[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Bind textures to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->maps[0], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->maps[1], 0);

        GLenum drawBuffers[] = {
            GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1
        };
        glDrawBuffers(2, drawBuffers);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE) {
            switch(status) {
                case GL_FRAMEBUFFER_UNDEFINED:
                    LOGE("Undefined framebuffer");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    LOGE("Incomplete attachment");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    LOGE("Missing attachment");
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    LOGE("Framebuffer unsupported");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    LOGE("Incomplete multisample");
                    break;
                default:
                    LOGE("Other framebuffer error");
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    Simulation::~Simulation() {}

    void Simulation::simulate(glm::vec3 translation) {
        glUseProgram(this->program);

        glUniform1f(this->uniformScale, this->scale);
        glUniform1f(this->uniformTime, (float) glfwGetTime() * 5.0f);
        glUniform2f(this->uniformTransition, translation.x, -translation.z);

        int prevViewport[4];
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
        glViewport(0, 0, this->width, this->height);

        Core::DrawContext(this->geometry);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);

        glUseProgram(0);
    }

    void Simulation::generateRandomWaves() {
        Random random(2137L);
        int waveCount = 10;
        float windAngle = random.nextFloat() * 100.0f;
        glm::vec2 windDirection = glm::vec2(cosf(windAngle), sinf(windAngle));

        if(windDirection.y < 0.0f)
            windAngle = -windAngle;

        std::string format("waves[i].X");

        glUseProgram(this->program);
        glUniform1i(this->uniformWaveCount, waveCount);

        for(int i=0; i<waveCount; ++i) {
            float amplitude = random.nextFloat() * 0.03f + 0.01f;
            float stepness = random.nextFloat() * 0.1f + 0.3f;
            float waveAngle = random.nextFloat() * glm::radians(2.0f * 27.0f) + (windAngle - glm::radians(27.0f));
            glm::vec2 waveDirection = glm::vec2(cosf(waveAngle), sinf(waveAngle));
            float speed = random.nextFloat() * 0.4f + 0.3f;
            float lenght = (random.nextFloat() * 30.0f + 30.0f) * amplitude;

            float w = sqrt(2.0f * 3.14159f * 9.8f / lenght);
            float f = 2.0f * speed / lenght;

            format[6] = i + '0';

            format[9] = 'A';
            glUniform1f(glGetUniformLocation(this->program, format.data()), amplitude);
            format[9] = 'Q';
            glUniform1f(glGetUniformLocation(this->program, format.data()), stepness);
            format[9] = 'D';
            glUniform2f(glGetUniformLocation(this->program, format.data()), waveDirection.x, waveDirection.y);
            format[9] = 'w';
            glUniform1f(glGetUniformLocation(this->program, format.data()), w);
            format[9] = 'f';
            glUniform1f(glGetUniformLocation(this->program, format.data()), f);
        }

        glUseProgram(0);
    }
}