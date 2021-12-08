#include <opengl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <cmath>
#include <vector>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

GLFWwindow *window;

GLuint programColor;
GLuint programTexture;

Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;

glm::vec3 cameraPos = glm::vec3(0, 0, 5);
glm::vec3 cameraDir; // Wektor "do przodu" kamery
glm::vec3 cameraSide; // Wektor "w bok" kamery
float cameraAngle = 0;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

glm::quat rotation = glm::quat(1, 0, 0, 0);

GLuint textureAsteroid;

void glfw_error_callback(int, const char *err_str)
{
    printf("GLFW Error: %s\n", err_str);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int modifier)
{
    printf("key: %d, scancode: %d, action: %d, modifier: %d\n", key, scancode, action, modifier);
    // if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    //    glfwSetWindowShouldClose(window, 1);
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
	switch(key)
	{
	case GLFW_KEY_Z: cameraAngle -= angleSpeed; break;
	case GLFW_KEY_X: cameraAngle += angleSpeed; break;
	case GLFW_KEY_W: cameraPos += cameraDir * moveSpeed; break;
	case GLFW_KEY_S: cameraPos -= cameraDir * moveSpeed; break;
	case GLFW_KEY_D: cameraPos += cameraSide * moveSpeed; break;
	case GLFW_KEY_A: cameraPos -= cameraSide * moveSpeed; break;
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

void drawObjectColor(Core::RenderContext context, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

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

void do_frame()
{
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.1f, 0.3f, 1.0f);

	glm::mat4 shipInitialTransformation = glm::translate(glm::vec3(0,-0.25f,0)) * glm::rotate(glm::radians(180.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f) * glm::rotate(-cameraAngle, glm::vec3(0,1,0)) * shipInitialTransformation;
	drawObjectColor(shipContext, shipModelMatrix, glm::vec3(0.6f));

	drawObjectTexture(sphereContext, glm::translate(glm::vec3(0,0,0)), textureAsteroid);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void loadModelToContext(std::string path, Core::RenderContext& context)
{
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
	srand(time(0));
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("assets/shaders/shader_color.vert", "assets/shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("assets/shaders/shader_tex.vert", "assets/shaders/shader_tex.frag");
	loadModelToContext("assets/models/spaceship.obj", shipContext);
	loadModelToContext("assets/models/sphere.obj", sphereContext);
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
        window = glfwCreateWindow(1280, 720, "GLFW test", NULL, NULL);
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
            printf("OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);
#endif
            init();
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