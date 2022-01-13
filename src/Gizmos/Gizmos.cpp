#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <ResourceLoader.hpp>
#include <Resources/Resources.hpp>
#include <Gizmos/Gizmos.hpp>
#include <Camera/Camera.hpp>

void Gizmos::init() {
    Gizmos::cubeRenderContext = Resources::MODELS.CUBE->meshes[0]->getRenderContext();
}

void Gizmos::cube(glm::vec3 position) {
    glm::vec3 size = glm::vec3(0.02f);
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    return Gizmos::cube(position, size, color);
}

void Gizmos::cube(glm::vec3 position, glm::vec3 size, glm::vec3 color) {
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
    Gizmos::drawWireframe(modelMatrix, Gizmos::cubeRenderContext, color);
}

void Gizmos::drawWireframe(glm::mat4 modelMatrix, Core::RenderContext* renderContext, glm::vec3 color) {
    glm::mat4 viewMatrix = camera->getTransformationMatrix();
    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;

    glUseProgram(resourceLoaderExternal->p_shader_color);
    glUniform3f(resourceLoaderExternal->p_shader_color_uni_objectColor, color.x, color.y, color.z);
    glUniformMatrix4fv(resourceLoaderExternal->p_shader_color_uni_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(resourceLoaderExternal->p_shader_color_uni_modelViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

    glBindVertexArray(renderContext->vertexArray);
	glDrawElements(
		GL_LINES,
		renderContext->size,
		GL_UNSIGNED_INT,
		(void*)0
	);
	glBindVertexArray(0);
}