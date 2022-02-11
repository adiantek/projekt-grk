#include <Logger.h>
#include <opengl.h>
#include <png/png.h>
#include <stdio.h>
#include <zlib/zlib.h>

#include <Camera/Camera.hpp>
#include <Camera/Scope.hpp>
#include <Controller/Controller.hpp>
#include <Fish/Barracuda.hpp>
#include <Fish/Boids.hpp>
#include <Fish/Cubefish.hpp>
#include <Fish/Golden.hpp>
#include <Fish/Pilotfish.hpp>
#include <Fish/RedSnapper.hpp>
#include <Fog/Fog.hpp>
#include <Glow/GlowShader.hpp>
#include <Physics/Physics.hpp>
#include <Physics/RigidBody.hpp>
#include <ResourceLoader.hpp>
#include <Resources/GameObject.hpp>
#include <Resources/Resources.hpp>
#include <Robot/Robot.hpp>
#include <Sound.hpp>
#include <Time/Time.hpp>
#include <Water/Water.hpp>
#include <glm/ext.hpp>
#include <utils/Gizmos.hpp>
#include <utils/Line.hpp>
#include <utils/MatrixTextureArray.hpp>
#include <vector>
#include <world/World.hpp>
#include <Glow/GlowShader.hpp>
#include <utils/Line.hpp>
#include <Physics/Physics.hpp>
#include <Physics/RigidBody.hpp>
#include <Fog/Fog.hpp>
#include <Particle/ParticleSystem.hpp>
#include <world/ChunkPosition.hpp>

#include "PxPhysics.h"
#include "Render_Utils.h"
#include "Texture.h"
#include "foundation/PxMathUtils.h"

#define BOIDS_AMOUNT 12
#define BOIDS_SIZE 8

// Window
GLFWwindow *window;
double lastTitleUpdate = 0.0;

// Other stuff...
glm::mat4 viewMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(-92.0f, 222.0f, -256.0f));
glm::vec3 lightPos = lightDir * 100000000.0f;

bool initialized = false;

ResourceLoader resourceLoader;

std::vector<fish::Boids<fish::Pilotfish> *> boids;
std::vector<fish::Boids<fish::Barracuda> *> boids2;
std::vector<fish::Boids<fish::RedSnapper> *> boids3;
std::vector<fish::Boids<fish::Golden> *> boids4;
std::vector<fish::Cubefish *> cubefish;

world::World *w;

void glfw_error_callback(int, const char *err_str) {
    LOGE("GLFW Error: %s", err_str);
}

void init();

void do_frame() {
    if (!resourceLoader.loadResources()) {
        return;
    }
    init();
    glfwPollEvents();
    // Update area
    timeExternal->update();
    physicsObject->update(timeExternal->deltaTime);
    controller->update();
    robot->update();
    camera->update();
    particleSystem->update();
    
    for (auto boid : boids)
        boid->update();
    for (auto boid : boids2)
        boid->update();
    for (auto boid : boids3)
        boid->update();
    for (auto boid : boids4)
        boid->update();
    for (auto cube : cubefish)
        cube->update();

    waterObject->update();

    w->update();

    fog->useFramebuffer();

    glUseProgram(resourceLoader.p_chunk);
    glUniform3f(resourceLoader.p_chunk_uni_lightPosition, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(resourceLoader.p_chunk_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);

    glUseProgram(resourceLoader.p_cubefish);
    glUniform3f(resourceLoader.p_cubefish_uni_lightPosition, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(resourceLoader.p_cubefish_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);

    glUseProgram(resourceLoader.p_pilotfish);
    glUniform3f(resourceLoader.p_pilotfish_uni_lightPosition, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(resourceLoader.p_pilotfish_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);
    glUniformMatrix4fv(resourceLoader.p_pilotfish_uni_lightTransformation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));

    glUseProgram(resourceLoader.p_normal_fish);
    glUniform3f(resourceLoader.p_normal_fish_uni_lightPosition, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(resourceLoader.p_normal_fish_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);
    glUniformMatrix4fv(resourceLoader.p_normal_fish_uni_lightTransformation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));

    glUseProgram(resourceLoader.p_instanced_kelp);
    glUniform3f(resourceLoader.p_instanced_kelp_uni_lightPosition, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(resourceLoader.p_instanced_kelp_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);
    glUniformMatrix4fv(resourceLoader.p_instanced_kelp_uni_lightTransformation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));

    glUseProgram(resourceLoader.p_rock);
    glUniform3f(resourceLoader.p_rock_uni_lightPosition, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(resourceLoader.p_rock_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);
    glUniformMatrix4fv(resourceLoader.p_rock_uni_lightTransformation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));

    glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Glow::glow->clear();
    // Draw area
    viewMatrix = camera->getTransformationMatrix();

    w->draw(viewMatrix);
    for (auto boid : boids)
        boid->draw(viewMatrix);
    for (auto boid : boids2)
        boid->draw(viewMatrix);
    for (auto boid : boids3)
        boid->draw(viewMatrix);
    for (auto boid : boids4)
        boid->draw(viewMatrix);
    for (auto cube : cubefish)
        cube->draw(viewMatrix);
    waterObject->draw(viewMatrix);
    physicsObject->draw(viewMatrix);

    particleSystem->render();

    fog->stopUsingFramebuffer();

    utils::Gizmos::draw();
    Glow::glow->draw(viewMatrix);
    scope->draw();

    if (timeExternal->lastFrame - lastTitleUpdate > 0.25) {
        lastTitleUpdate = timeExternal->lastFrame;
        char title[200];
        sprintf(title, "FPS: %u, Collected coins: %d frustum: %d / %d (rosliny: %d / %d)", timeExternal->fps,
                robot->collectedCoins,
                w->frustumDraw, w->frustumTotal,
                w->frustumDrawDecorator, w->frustumTotal);
        glfwSetWindowTitle(window, title);
    }
    glfwSwapBuffers(window);
}

void onChunkLoad(Random *random, world::ChunkPosition pos) {
    int minX = pos.coords.x << 4;
    int minZ = pos.coords.z << 4;

    if (random->nextFloat() < 0.02f) {
        fish::Boids<fish::Pilotfish> *boid = new fish::Boids<fish::Pilotfish>(BOIDS_SIZE, glm::vec3(minX + random->nextFloat() * 16.0f, random->nextFloat(170.0f, 190.0f), minZ + random->nextFloat() * 16.0f), w);
        waterObject->addWorldObject((world::Object3D *)boid);
        boids.push_back(boid);
    }
    
    if (random->nextFloat() < 0.02f) {
        fish::Boids<fish::Barracuda> *boid = new fish::Boids<fish::Barracuda>(BOIDS_SIZE, glm::vec3(minX + random->nextFloat() * 16.0f, random->nextFloat(170.0f, 190.0f), minZ + random->nextFloat() * 16.0f), w);
        waterObject->addWorldObject((world::Object3D *)boid);
        boids2.push_back(boid);
    }
    
    if (random->nextFloat() < 0.02f) {
        fish::Boids<fish::RedSnapper> *boid = new fish::Boids<fish::RedSnapper>(BOIDS_SIZE, glm::vec3(minX + random->nextFloat() * 16.0f, random->nextFloat(170.0f, 190.0f), minZ + random->nextFloat() * 16.0f), w);
        waterObject->addWorldObject((world::Object3D *)boid);
        boids3.push_back(boid);
    }
    
    if (random->nextFloat() < 0.02f) {
        fish::Boids<fish::Golden> *boid = new fish::Boids<fish::Golden>(BOIDS_SIZE, glm::vec3(minX + random->nextFloat() * 16.0f, random->nextFloat(170.0f, 190.0f), minZ + random->nextFloat() * 16.0f), w);
        waterObject->addWorldObject((world::Object3D *)boid);
        boids4.push_back(boid);
    }
}

void init() {
    if (initialized) return;
    initialized = true;

    // Initialize resources (textures, shaders, materials)
    Resources::init();
    new Scope();
    sound->start();

    new Glow::GlowShader(camera->width, camera->height);

    fog = new Fog(camera->width, camera->height, 256.0);
    new water::Water(192.0f, 320.0f, 65.0f, 400, 300.0f, 1200);
    new physics::Physics(9.8f);
    w = new world::World(0, onChunkLoad);

    new ParticleSystem();

    waterObject->addWorldObject((world::Object3D *)w);

    physicsObject->world = w;

    glEnable(GL_DEPTH_TEST);

    Random random(0L);
    cubefish.push_back(new fish::Cubefish(glm::vec3(24.0f, 400.0f, -95.0f), 1.0f, 0.8f));
    cubefish.push_back(new fish::Cubefish(glm::vec3(27.0f, 400.0f, -95.0f), 3.0f, 2.4f));
    waterObject->addWorldObject((world::Object3D *)cubefish[0]);
    waterObject->addWorldObject((world::Object3D *)cubefish[1]);
}

#ifndef EMSCRIPTEN
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    LOGE("GL CALLBACK [%u]: %s type = 0x%x, source = 0x%x, severity = 0x%x, message = %s", id, (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, source, severity, message);
}
#endif

int main(int argc, char **argv) {
    vertex::VertexFormats_load();
    argv++;
    argc--;
    new Sound(&argv, &argc);
    LOGD("ZLIB version: %s", zlibVersion());
    LOGD("libpng version: %s", png_get_libpng_ver(NULL));
    {
        physx::PxTransform transform = PxTransformFromPlaneEquation(physx::PxPlane(1, 1, 0, 2));
        LOGD("Physics success: %f %f %f", transform.p.x, transform.p.y, transform.p.z);
    }

    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() != GL_TRUE) {
        LOGE("glfwInit() failed");
        glfwTerminate();
        return EXIT_FAILURE;
    } else {
        LOGI("glfwInit() success");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(1280, 720, "Lubię trójkąty", NULL, NULL);
        if (!window) {
            LOGE("glfwCreateWindow() failed");
            glfwTerminate();
            return EXIT_FAILURE;
        } else {
            LOGI("glfwCreateWindow() success");
            glfwMakeContextCurrent(window);
            glfwSwapInterval(0);
            new Time();
            new Controller(window);
            new Camera(1280, 720);
#ifndef EMSCRIPTEN
            if (!gladLoadGL()) {
                LOGE("gladLoadGL() failed");
                return EXIT_FAILURE;
            }
            // stupid fix for gladLoadGL()
            for (int i = 0; i < 100; i++) {
                if (!glGetError())
                    break;
            }
            LOGI("OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);
            glEnable(GL_DEBUG_OUTPUT);

            // disable NVIDIA notification about using VIDEO memory
            glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);

            glDebugMessageCallback(MessageCallback, 0);
#endif
#ifdef EMSCRIPTEN
            emscripten_set_main_loop(do_frame, 0, 1);
#else
            while (!glfwWindowShouldClose(window)) {
                do_frame();
            }
#endif
        }
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}