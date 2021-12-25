#include<Water/Simulation.hpp>

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Init normal map texture
        glBindTexture(GL_TEXTURE_2D, this->maps[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Bind textures to framebuffer
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->maps[0], 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->maps[1], 0);

        GLenum drawBuffers[] = {
            GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1
        };
        glDrawBuffers(2, drawBuffers);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glUseProgram(this->program);
        float r = randf(0.0, 100.0);
        glm::vec2 windDir = glm::vec2(cosf(r), sinf(r));
        int waveCount = 10;
        glUniform1i(this->uniformWaveCount, waveCount);
        glUniform1f(this->uniformScale, scale / 2.0f);
        for (int i = 0; i < waveCount; ++i) {
            float x = randf(0.01f, 0.04f);
            std::cout << "waves[" << i << "].A = " << x << ";\n";
            float q = randf(0.3f, 0.4f);
            std::cout << "waves[" << i << "].Q = " << q << ";\n";
            float windAngle = acosf((windDir.x/sqrtf(windDir.x*windDir.x + windDir.y*windDir.y)));
            if (windDir.y < 0) windAngle = -windAngle;
            float waveAngle = randf(windAngle - glm::radians(27.0f),
                                    windAngle + glm::radians(27.0f));
            std::cout << "waves[" << i << "].D = vec2(" << cos(waveAngle) << ", " << sin(waveAngle) << ");\n";
            float s = randf(0.3f, 0.7f);
            std::cout << "waves[" << i << "].s = " << s << ";\n";
            float l = randf(30.0f, 60.0f) * x;
            std::cout << "waves[" << i << "].l = " << l << ";\n";
            std::ostringstream out;
            out << "waves[" << i << "].A";
            glUniform1f(glGetUniformLocation(this->program, out.str().c_str()), x);
            std::ostringstream out1;
            out1 << "waves[" << i << "].Q";
            glUniform1f(glGetUniformLocation(this->program, out1.str().c_str()), q);
            std::ostringstream out2;
            out2 << "waves[" << i << "].D";
            glUniform2f(glGetUniformLocation(this->program, out2.str().c_str()), cos(waveAngle), sin(waveAngle));
            std::ostringstream out3;
            out3 << "waves[" << i << "].w";
            glUniform1f(glGetUniformLocation(this->program, out3.str().c_str()), sqrt(2.0f * 3.14f * 9.8f / l));
            std::ostringstream out4;
            out4 << "waves[" << i << "].fi";
            glUniform1f(glGetUniformLocation(this->program, out4.str().c_str()), 2.0f * s / l);
        }
        glUseProgram(0);
    }
    
    Simulation::~Simulation() {}

    void Simulation::simulate(glm::vec3 translation) {
        glUseProgram(this->program);

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
}