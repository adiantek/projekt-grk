#include <opengl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <stdbool.h>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

GLFWwindow *window;

GLuint programColor;
GLuint programTexture;

GLuint program;
GLuint programSun;
GLuint programTex;

Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;
Core::RenderContext sphereContext2;

glm::vec3 cameraPos = glm::vec3(0, 0, 5);
glm::vec3 cameraDir; // Wektor "do przodu" kamery
glm::vec3 cameraSide; // Wektor "w bok" kamery
float cameraAngle = 0;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

glm::quat rotation = glm::quat(1, 0, 0, 0);

GLuint textureAsteroid;
bool keyPressed[] = {0, 0, 0, 0, 0, 0};
bool initialized = false;

namespace texture {
	GLuint grid;
	GLuint gridColor;
	GLuint earth;
	GLuint earth2;
	GLuint moon;
	GLuint ship;

	GLuint earthNormal;
	GLuint asteroidNormal;
	GLuint shipNormal;
}

void glfw_error_callback(int, const char *err_str)
{
    printf("GLFW Error: %s\n", err_str);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int modifier)
{
    printf("key: %d, scancode: %d, action: %d, modifier: %d\n", key, scancode, action, modifier);
	switch (key)
	{
	case GLFW_KEY_Z: keyPressed[0] = action != 0; break;
	case GLFW_KEY_X: keyPressed[1] = action != 0; break;
	case GLFW_KEY_W: keyPressed[2] = action != 0; break;
	case GLFW_KEY_S: keyPressed[3] = action != 0; break;
	case GLFW_KEY_D: keyPressed[4] = action != 0; break;
	case GLFW_KEY_A: keyPressed[5] = action != 0; break;
	}
}

static void wmbutcb(GLFWwindow *window, int button, int action, int mods)
{
    //assert(window != NULL); (void)button; (void)action; (void)mods;
    printf("mouse button: %d, action: %d, mods: %d\n", button, action, mods);
}

glm::mat4 createCameraMatrix()
{
	cameraDir = glm::vec3(cosf(cameraAngle - glm::radians(90.0f)), 0.0f, sinf(cameraAngle - glm::radians(90.0f)));
	glm::vec3 up = glm::vec3(0, 1, 0);
	cameraSide = glm::cross(cameraDir, up);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectColor(GLuint program, Core::RenderContext context, glm::mat4 modelMatrix, glm::vec3 color)
{
	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawContext(context);

	glUseProgram(0);
}

void drawObjectTexture(Core::RenderContext context, glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	Core::SetActiveTexture(textureId, "textureSampler", program, 0);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawContext(context);

	glUseProgram(0);
}

void drawObjectTexNormal(GLuint programTex, Core::RenderContext context, glm::mat4 modelMatrix, GLuint txt, GLuint txtNormal)
{
	glUseProgram(programTex);
	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	Core::SetActiveTexture(txt, "colorTexture", programTex, 0);
	Core::SetActiveTexture(txtNormal, "normalSampler", programTex, 1);
	glUniformMatrix4fv(glGetUniformLocation(programTex, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(programTex, "transformation"), 1, GL_FALSE, (float*)&transformation);
	Core::DrawContext(context);
	glUseProgram(0);
}

void process_keys() {
	float angleSpeed = 0.05f;
	float moveSpeed = 0.05f;
	if (keyPressed[0]) {
		cameraAngle -= angleSpeed;
	}
	if (keyPressed[1]) {
		cameraAngle += angleSpeed;
	}
	if (keyPressed[2]) {
		cameraPos += cameraDir * moveSpeed;
	}
	if (keyPressed[3]) {
		cameraPos -= cameraDir * moveSpeed;
	}
	if (keyPressed[4]) {
		cameraPos += cameraSide * moveSpeed;
	}
	if (keyPressed[5]) {
		cameraPos -= cameraSide * moveSpeed;
	}
}

void do_frame()
{
	init();
	process_keys();
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.1f, 0.3f, 1.0f);

	glm::mat4 shipInitialTransformation = glm::translate(glm::vec3(0,-0.25f,0)) * glm::rotate(glm::radians(180.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f) * glm::rotate(-cameraAngle, glm::vec3(0,1,0)) * shipInitialTransformation;
	// drawObjectColor(programColor, shipContext, shipModelMatrix, glm::vec3(0.6f));

	drawObjectColor(programSun, sphereContext2, glm::translate(glm::vec3(0,0,0)), glm::vec3(0.6f));

	double time = glfwGetTime();
	glm::vec3 lightPos = glm::vec3(0, 0, 0);
	glUseProgram(programTex);
	glUniform3f(glGetUniformLocation(programTex, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(programTex, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
	glUseProgram(programSun);
	glUniform3f(glGetUniformLocation(programSun, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(programSun, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	drawObjectTexNormal(programTex, shipContext, shipModelMatrix, texture::ship, texture::shipNormal);
	glm::mat4 eu = glm::eulerAngleY(time / 2.0);
	eu = glm::translate(eu, glm::vec3(-5, 0, 0));
	
	drawObjectTexNormal(programTex, sphereContext, eu * glm::scale(glm::vec3(0.7f)), texture::earth, texture::earthNormal);
	drawObjectTexNormal(programTex, sphereContext, eu * glm::translate(glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(0.2f)), texture::moon, texture::asteroidNormal);

	drawObjectColor(programSun, sphereContext2, glm::translate(lightPos), glm::vec3(1.0f, 0.8f, 0.2f));

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void loadModelToContext(std::string path, Core::RenderContext& context)
{
	printf("Loading model %s...\n", path.c_str());
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	context.initFromAssimpMesh(scene->mMeshes[0]);
}

void init()
{
	if (initialized) {
		return;
	}
	initialized = true;
	srand(time(0));
	glEnable(GL_DEPTH_TEST);

	texture::grid = Core::LoadTexture("assets/textures/grid.png");
	texture::gridColor = Core::LoadTexture("assets/textures/grid_color.png");
	texture::earth = Core::LoadTexture("assets/textures/earth.png");
	texture::earth2 = Core::LoadTexture("assets/textures/earth2.png");
	texture::moon = Core::LoadTexture("assets/textures/moon.png");
	texture::ship = Core::LoadTexture("assets/textures/spaceship.png");
	texture::earthNormal = Core::LoadTexture("assets/textures/earth2_normals.png");
	texture::asteroidNormal = Core::LoadTexture("assets/textures/moon_normals.png");
	texture::shipNormal = Core::LoadTexture("assets/textures/spaceship_normals.png");

	programColor = shaderLoader.CreateProgram("assets/shaders/shader_color.vert", "assets/shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("assets/shaders/shader_tex.vert", "assets/shaders/shader_tex.frag");
	
	program = shaderLoader.CreateProgram("assets/shaders/shader_4_1.vert", "assets/shaders/shader_4_1.frag");
	programSun = shaderLoader.CreateProgram("assets/shaders/shader_4_sun.vert", "assets/shaders/shader_4_sun.frag");
	programTex = shaderLoader.CreateProgram("assets/shaders/shader_4_tex.vert", "assets/shaders/shader_4_tex.frag");

	loadModelToContext("assets/models/spaceship.obj", shipContext);
	loadModelToContext("assets/models/sphere.obj", sphereContext);
	loadModelToContext("assets/models/sphere2.obj", sphereContext2);
	textureAsteroid = Core::LoadTexture("assets/textures/asteroid.png");
}

int main(int argc, char **argv)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (glfwInit() != GL_TRUE)
    {
        printf("glfwInit() failed\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    else
    {
        printf("glfwInit() success\n");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(600, 600, "GLFW test", NULL, NULL);
        if (!window)
        {
            printf("glfwCreateWindow() failed\n");
            glfwTerminate();
            return EXIT_FAILURE;
        }
        else
        {
            printf("glfwCreateWindow() success\n");
            glfwMakeContextCurrent(window);
            // glfwSetWindowSizeCallback(window, window_size_callback);
            glfwSetMouseButtonCallback(window, wmbutcb);
            glfwSetKeyCallback(window, key_callback);
#ifndef EMSCRIPTEN
            if (!gladLoadGL())
            {
                printf("gladLoadGL() failed\n");
                return EXIT_FAILURE;
            }
            printf("OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);
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