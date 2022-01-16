#include "Texture.h"

#include <Logger.h>

#include "SOIL/SOIL.h"

void Core::SetActiveTexture(GLuint textureID, const char* shaderVariableName, GLuint programID, int textureUnit) {
    glUniform1i(glGetUniformLocation(programID, shaderVariableName), textureUnit);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}
