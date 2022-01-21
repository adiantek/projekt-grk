#include <opengl.h>
#include <Logger.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <stdbool.h>

#include <ResourceLoader.hpp>
#include <Controller/Controller.hpp>
#include <Water/Water.hpp>
#include <Random.hpp>
#include <Camera/Camera.hpp>
#include <Time/Time.hpp>
#include <vertex/VertexFormats.hpp>

#include <Resources/Resources.hpp>
#include <Robot/Robot.hpp>
#include <Resources/GameObject.hpp>
#include <world/World.hpp>
#include <Physics/Physics.hpp>
#include <Physics/RigidBody.hpp>

#include "Render_Utils.h"
#include "Texture.h"

#include <zlib/zlib.h>
#include <png/png.h>
#include "PxPhysics.h"
#include "foundation/PxMathUtils.h"

Core::RenderContext sphereContext;
Core::RenderContext sphereContext2;
Core::RenderContext brickWallContext;
Core::RenderContext planeContext;

// Temporary ground plane
GameObject *ground = new GameObject("ground");

// Window
GLFWwindow *window;

// Other stuff...
glm::mat4 viewMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(-92.0f, 222.0f, -256.0f));
glm::vec3 lightPos = lightDir * 100000000.0f;

bool initialized = false;

ResourceLoader resourceLoader;

physics::RigidBody* rigidBody;
physics::RigidBody* rigidBodyHeavy;

world::World *w;

void glfw_error_callback(int, const char *err_str)
{
    LOGE("GLFW Error: %s", err_str);
}

void drawObjectColor(Core::RenderContext context, glm::mat4 modelMatrix, glm::vec3 color)
{
	glUseProgram(resourceLoader.p_shader_4_sun);

	glUniform3f(resourceLoader.p_shader_4_sun_uni_objectColor, color.x, color.y, color.z);
	// glUniform3f(resourceLoader.p_shader_4_sun_uni_ligh, lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = viewMatrix * modelMatrix;
	glUniformMatrix4fv(resourceLoader.p_shader_4_sun_uni_modelViewProjectionMatrix, 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(resourceLoader.p_shader_4_sun_uni_modelMatrix, 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawContext(context);
}

void drawObjectTexture(Core::RenderContext context, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = resourceLoader.p_shader_tex;

	glUseProgram(program);

	Core::SetActiveTexture(textureId, "colorTexture", program, 0);

	glm::mat4 transformation = viewMatrix * modelMatrix;
	glUniformMatrix4fv(resourceLoader.p_shader_tex_uni_modelViewProjectionMatrix, 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(resourceLoader.p_shader_tex_uni_modelMatrix, 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawContext(context);
}

void drawObjectTexNormal(Core::RenderContext context, glm::mat4 modelMatrix, GLuint txt, GLuint txtNormal)
{
	glUseProgram(resourceLoader.p_shader_4_tex);
	glm::mat4 transformation = viewMatrix * modelMatrix;
	Core::SetActiveTexture(txt, "colorTexture", resourceLoader.p_shader_4_tex, 0);
	Core::SetActiveTexture(txtNormal, "normalSampler", resourceLoader.p_shader_4_tex, 1);
	glUniformMatrix4fv(resourceLoader.p_shader_4_tex_uni_modelMatrix, 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(resourceLoader.p_shader_4_tex_uni_transformation, 1, GL_FALSE, (float*)&transformation);
	Core::DrawContext(context);
	glUseProgram(0);
}

void drawObjectTexNormalCaustics(Core::RenderContext context, glm::mat4 modelMatrix, GLuint txt, GLuint txtNormal, GLuint txtHeight = 0)
{
	glUseProgram(resourceLoader.p_chunk);
	glm::mat4 transformation = viewMatrix * modelMatrix;
	glm::mat4 transformationLight = waterObject->getLightCamera();
	Core::SetActiveTexture(txt, "colorTexture", resourceLoader.p_chunk, 0);
	Core::SetActiveTexture(txtNormal, "normalSampler", resourceLoader.p_chunk, 1);
	Core::SetActiveTexture(waterObject->getCausticsMap(), "caustics", resourceLoader.p_chunk, 2);
	Core::SetActiveTexture(txtHeight, "depthMap", resourceLoader.p_chunk, 3);
	glUniformMatrix4fv(resourceLoader.p_chunk_uni_modelMatrix, 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(resourceLoader.p_chunk_uni_transformation, 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(resourceLoader.p_chunk_uni_lightTransformation, 1, GL_FALSE, (float*)&transformationLight);
	Core::DrawContext(context);
	glUseProgram(0);
}

void drawObjectTexNormalParallax(Core::RenderContext context, glm::mat4 modelMatrix, GLuint txt, GLuint txtNormal, GLuint txtHeight)
{
	glEnable(GL_CULL_FACE);
	glUseProgram(resourceLoader.p_shader_4_tex_with_parallax);
	glm::mat4 transformation = viewMatrix * modelMatrix;
	Core::SetActiveTexture(txt, "colorTexture", resourceLoader.p_shader_4_tex_with_parallax, 0);
	Core::SetActiveTexture(txtNormal, "normalSampler", resourceLoader.p_shader_4_tex_with_parallax, 1);
	Core::SetActiveTexture(txtHeight, "depthSampler", resourceLoader.p_shader_4_tex_with_parallax, 2);
	glUniformMatrix4fv(resourceLoader.p_shader_4_tex_with_parallax_uni_modelMatrix, 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(resourceLoader.p_shader_4_tex_with_parallax_uni_transformation, 1, GL_FALSE, (float*)&transformation);
	Core::DrawContext(context);
	glUseProgram(0);
	glDisable(GL_CULL_FACE);
}

void init();

Random r(0);

void do_frame()
{
	if (!resourceLoader.loadResources()) {
		return;
	}
	init();
	glfwPollEvents();
	
	timeExternal->update();
	physicsObject->update(timeExternal->deltaTime);
	controller->update();
	robot->update();
	camera->update();

	viewMatrix = camera->getTransformationMatrix();

	glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 eu = glm::eulerAngleY((float)timeExternal->lastFrame / 2.0);
	glm::mat4 eu2 = glm::eulerAngleY(2.5 / 2.0);
	eu = glm::translate(eu, glm::vec3(-5, 0, 0));

	waterObject->useFramebuffer();
	waterObject->drawObject(sphereContext, eu * glm::scale(glm::vec3(0.7f)));
	waterObject->drawObject(sphereContext, eu * glm::translate(glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(0.2f)));
	waterObject->drawObject(brickWallContext, glm::translate(glm::vec3(-10, 2, 0)) * eu2 * glm::scale(glm::vec3(1.0f)));
	waterObject->drawObject(brickWallContext, glm::translate(glm::vec3(-9, -2, 0)) * eu2 * glm::scale(glm::vec3(1.0f)));
	waterObject->drawObject(sphereContext2, glm::translate(lightPos));
	waterObject->drawObject(brickWallContext, glm::translate(glm::vec3(20, 126, -15)) * glm::eulerAngleY((float)timeExternal->lastFrame / 2.0f) * glm::scale(glm::vec3(1.0f)));
	waterObject->drawObject(brickWallContext, rigidBody->getModelMatrix());
	waterObject->drawObject(brickWallContext, rigidBodyHeavy->getModelMatrix());
	waterObject->stopUsingFramebuffer();

	waterObject->update();
	
	w->update();
	w->draw(viewMatrix);

	// ground->draw();
	glUseProgram(resourceLoader.p_shader_tex);
	glUniform3f(resourceLoader.p_shader_tex_uni_lightDir, lightDir.x, lightDir.y, lightDir.z);

	glUseProgram(resourceLoader.p_shader_4_tex);
	glUniform3f(resourceLoader.p_shader_4_tex_uni_lightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(resourceLoader.p_shader_4_tex_uni_cameraPos, camera->position.x, camera->position.y, camera->position.z);

	glUseProgram(resourceLoader.p_shader_4_tex_with_parallax);
	glUniform3f(resourceLoader.p_shader_4_tex_with_parallax_uni_lightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(resourceLoader.p_shader_4_tex_with_parallax_uni_cameraPos, camera->position.x, camera->position.y, camera->position.z);
	glUniform1f(resourceLoader.p_shader_4_tex_with_parallax_uni_heightScale, 0.06f);

	glUseProgram(resourceLoader.p_chunk);
	glUniform3f(resourceLoader.p_chunk_uni_lightPosition, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(resourceLoader.p_chunk_uni_cameraPosition, camera->position.x, camera->position.y, camera->position.z);

	glUseProgram(resourceLoader.p_shader_4_1);
	glUniform3f(resourceLoader.p_shader_4_1_uni_lightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(resourceLoader.p_shader_4_1_uni_cameraPos, camera->position.x, camera->position.y, camera->position.z);

	glUseProgram(resourceLoader.p_shader_4_sun);
	glUniform3f(resourceLoader.p_shader_4_sun_uni_cameraPos, camera->position.x, camera->position.y, camera->position.z);
	
	drawObjectTexNormalCaustics(sphereContext, eu * glm::scale(glm::vec3(0.7f)), resourceLoader.tex_earth, resourceLoader.tex_earthNormal);
	drawObjectTexNormalCaustics(sphereContext, eu * glm::translate(glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(0.2f)), resourceLoader.tex_moon, resourceLoader.tex_asteroidNormal);
	drawObjectTexNormalParallax(brickWallContext, glm::translate(glm::vec3(-10, 2, 0)) * eu2 * glm::scale(glm::vec3(1.0f)), resourceLoader.tex_wall, resourceLoader.tex_wallNormal, resourceLoader.tex_wallHeight);
	drawObjectTexNormalCaustics(brickWallContext, glm::translate(glm::vec3(20, 126, -15)) * glm::eulerAngleY((float)timeExternal->lastFrame / 2.0f) * glm::scale(glm::vec3(1.0f)), resourceLoader.tex_wall, resourceLoader.tex_wallNormal, resourceLoader.tex_wallHeight);
	drawObjectTexNormalCaustics(brickWallContext, rigidBody->getModelMatrix(), resourceLoader.tex_wall, resourceLoader.tex_wallNormal, resourceLoader.tex_wallHeight);
	drawObjectTexNormalCaustics(brickWallContext, rigidBodyHeavy->getModelMatrix(), resourceLoader.tex_wall, resourceLoader.tex_wallNormal, resourceLoader.tex_wallHeight);
	drawObjectTexNormalCaustics(brickWallContext, glm::translate(glm::vec3(-8, -2, 0)) * eu2 * glm::scale(glm::vec3(1.0f)), resourceLoader.tex_wall, resourceLoader.tex_wallNormal);

	drawObjectColor(sphereContext2, glm::translate(lightPos), glm::vec3(1.0f, 0.8f, 0.2f));

	
	waterObject->draw(viewMatrix);

    glfwSwapBuffers(window);
}

void loadModelToContext(std::string path, Core::RenderContext& context)
{
	LOGI("Loading model %s...", path.c_str());
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOGE("ERROR::ASSIMP::%s", import.GetErrorString());
		return;
	}
	context.initFromAssimpMesh(scene->mMeshes[0]);
}

void init() {
	if (initialized) return;
	initialized = true;
	w = new world::World(0);
	new physics::Physics(9.8f, w);

	physx::PxTransform initPose = physx::PxTransform(24.0f, 400.0f, -95.0f);
	world::Object3D* objectDummy = new world::Object3D();
	rigidBody = new physics::RigidBody(false, initPose, physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), objectDummy, 0.5f, 0.5f, 0.001f);
	rigidBody->setMass(1.0f);
	rigidBody->density = 0.8f;


	initPose = physx::PxTransform(27.0f, 400.0f, -95.0f);
	rigidBodyHeavy = new physics::RigidBody(false, initPose, physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), objectDummy, 0.5f, 0.5f, 0.001f);
	rigidBodyHeavy->setMass(3.0f);
	rigidBodyHeavy->density = 0.8f * 3.0f;

	// Initialize resources (textures, shaders, materials)
	Resources::init();

	// Basic 
	ground
		->setModel(Resources::MODELS.PLANE)
		->setMaterial(Resources::MATERIALS.DEFAULT)
		->setPosition(glm::vec3(0, 0.0f, 0))
		->setScale(glm::vec3(10, 10, 10));

	glEnable(GL_DEPTH_TEST);

	loadModelToContext("assets/models/sphere.obj", sphereContext);
	loadModelToContext("assets/models/sphere2.obj", sphereContext2);
	loadModelToContext("assets/models/primitives/cube.obj", brickWallContext);
	planeContext.initPlane(2.0f, 2.0f);

	new water::Water(192.0f, 256.0f, 40.0f, 400, 256.0f, 1000);
	waterObject->addWorldObject((world::Object3D*) w);
}

int main(int argc, char **argv)
{
	vertex::VertexFormats_load();
	LOGD("ZLIB version: %s", zlibVersion());
	LOGD("libpng version: %s", png_get_libpng_ver(NULL));
	{
		physx::PxTransform transform = PxTransformFromPlaneEquation(physx::PxPlane(1, 1, 0, 2));
		LOGD("Physics success: %f %f %f", transform.p.x, transform.p.y, transform.p.z);
	}

	glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() != GL_TRUE)
    {
        LOGE("glfwInit() failed");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    else
    {
        LOGI("glfwInit() success");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(1280, 720, "Lubię trójkąty", NULL, NULL);
        if (!window)
        {
            LOGE("glfwCreateWindow() failed");
            glfwTerminate();
            return EXIT_FAILURE;
        }
        else
        {
            LOGI("glfwCreateWindow() success");
            glfwMakeContextCurrent(window);
			glfwSwapInterval(0);
			new Time();
			new Controller(window);
			new Camera(1280, 720);
#ifndef EMSCRIPTEN
            if (!gladLoadGL())
            {
                LOGE("gladLoadGL() failed");
                return EXIT_FAILURE;
            }
            LOGI("OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);
#endif
#ifdef EMSCRIPTEN
            emscripten_set_main_loop(do_frame, 0, 1);
#else
            while (!glfwWindowShouldClose(window))
            {
                do_frame();
            }
#endif
        }
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}