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
#include <Water/Surface.hpp>
#include <Random.hpp>
#include <SimplexNoiseGenerator.hpp>
#include <Robot/Robot.hpp>
#include <Camera/Camera.hpp>
#include <Time/Time.hpp>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Texture.h"

// Core elements
Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;
Core::RenderContext sphereContext2;
Core::RenderContext brickWallContext;
Core::RenderContext planeContext;

// Window
GLFWwindow *window;

// Other stuff...
glm::mat4 viewMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

bool initialized = false;

ResourceLoader resourceLoader;

Water::Surface* waterSurface;

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

	glUniform3f(resourceLoader.p_shader_tex_uni_lightDir, lightDir.x, lightDir.y, lightDir.z);
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

void drawObjectTexNormalParallax(Core::RenderContext context, glm::mat4 modelMatrix, GLuint txt, GLuint txtNormal, GLuint txtHeight)
{
	glUseProgram(resourceLoader.p_shader_4_tex_with_parallax);
	glm::mat4 transformation = viewMatrix * modelMatrix;
	Core::SetActiveTexture(txt, "colorTexture", resourceLoader.p_shader_4_tex_with_parallax, 0);
	Core::SetActiveTexture(txtNormal, "normalSampler", resourceLoader.p_shader_4_tex_with_parallax, 1);
	Core::SetActiveTexture(txtHeight, "depthSampler", resourceLoader.p_shader_4_tex_with_parallax, 2);
	glUniformMatrix4fv(resourceLoader.p_shader_4_tex_with_parallax_uni_modelMatrix, 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(resourceLoader.p_shader_4_tex_with_parallax_uni_transformation, 1, GL_FALSE, (float*)&transformation);
	Core::DrawContext(context);
	glUseProgram(0);
}

void init();

Random r(0);
SimplexNoiseGenerator *noise;

void do_frame()
{
	if (!resourceLoader.loadResources()) {
		return;
	}
	init();
	glfwPollEvents();
	
	timeExternal->update();

	controller->update();
	robot->update();
	camera->update();

	viewMatrix = camera->getTransformationMatrix();

	glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 shipInitialTransformation = glm::translate(glm::vec3(0,-0.25f,0)) * glm::rotate(glm::radians(180.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	glm::mat4 shipModelMatrix = glm::translate(robot->position) * shipInitialTransformation;

	double time = glfwGetTime();
	glm::vec3 lightPos = glm::vec3(0, 0, 0);

	glUseProgram(resourceLoader.p_shader_4_tex);
	glUniform3f(resourceLoader.p_shader_4_tex_uni_lightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(resourceLoader.p_shader_4_tex_uni_cameraPos, camera->position.x, camera->position.y, camera->position.z);

	glUseProgram(resourceLoader.p_shader_4_tex_with_parallax);
	glUniform3f(resourceLoader.p_shader_4_tex_with_parallax_uni_lightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(resourceLoader.p_shader_4_tex_with_parallax_uni_cameraPos, camera->position.x, camera->position.y, camera->position.z);
	glUniform1f(resourceLoader.p_shader_4_tex_with_parallax_uni_heightScale, 0.06f);

	glUseProgram(resourceLoader.p_shader_4_1);
	glUniform3f(resourceLoader.p_shader_4_1_uni_lightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(resourceLoader.p_shader_4_1_uni_cameraPos, camera->position.x, camera->position.y, camera->position.z);

	glUseProgram(resourceLoader.p_shader_4_sun);
	glUniform3f(resourceLoader.p_shader_4_sun_uni_cameraPos, camera->position.x, camera->position.y, camera->position.z);

	drawObjectTexNormal(shipContext, shipModelMatrix, resourceLoader.txt_ship, resourceLoader.txt_shipNormal);
	glm::mat4 eu = glm::eulerAngleY(time / 2.0);
	glm::mat4 eu2 = glm::eulerAngleY(2.5 / 2.0);
	eu = glm::translate(eu, glm::vec3(-5, 0, 0));
	
	drawObjectTexNormal(sphereContext, eu * glm::scale(glm::vec3(0.7f)), resourceLoader.txt_earth, resourceLoader.txt_earthNormal);
	drawObjectTexNormal(sphereContext, eu * glm::translate(glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(0.2f)), resourceLoader.txt_moon, resourceLoader.txt_asteroidNormal);
	drawObjectTexNormalParallax(planeContext, glm::translate(glm::vec3(-8, 0, 0)) * eu2 * glm::scale(glm::vec3(1.0f)), resourceLoader.txt_wall, resourceLoader.txt_wallNormal, resourceLoader.txt_wallHeight);
	drawObjectTexNormalParallax(brickWallContext, glm::translate(glm::vec3(-8, 2, 0)) * eu2 * glm::scale(glm::vec3(1.0f)), resourceLoader.txt_wall, resourceLoader.txt_wallNormal, resourceLoader.txt_wallHeight);
	drawObjectTexNormal(brickWallContext, glm::translate(glm::vec3(-8, -2, 0)) * eu2 * glm::scale(glm::vec3(1.0f)), resourceLoader.txt_wall, resourceLoader.txt_wallNormal);
	waterSurface->draw(viewMatrix, camera->position);

	drawObjectColor(sphereContext2, glm::translate(lightPos), glm::vec3(1.0f, 0.8f, 0.2f));
	
	// double st = glfwGetTime();
	// for (int i = 0; i < 1000; i++)
	// 	noise->draw(&resourceLoader);
	// st = glfwGetTime() - st;
	// LOGD("time: %.3f", st);

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

void init()
{
	if (initialized) {
		return;
	}

	// Basic 
	new Camera(600, 600);
	new Robot();

	initialized = true;
	glEnable(GL_DEPTH_TEST);
	noise = new SimplexNoiseGenerator(&r, &resourceLoader);


	loadModelToContext("assets/models/spaceship.obj", shipContext);
	loadModelToContext("assets/models/sphere.obj", sphereContext);
	loadModelToContext("assets/models/sphere2.obj", sphereContext2);
	loadModelToContext("assets/models/primitives/cube.obj", brickWallContext);
	planeContext.initPlane(2.0f, 2.0f);

	waterSurface = new Water::Surface(0.0f, -9.0f, 0.0f, 25.0f, 25.0f, 256, 256, &resourceLoader);
	waterSurface->simulation.generateRandomWaves();
}

int main(int argc, char **argv)
{
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
        window = glfwCreateWindow(600, 600, "GLFW test", NULL, NULL);
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
			new Time();
			new Controller(window);
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