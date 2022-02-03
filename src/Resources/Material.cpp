#include <opengl.h>
#include <glm/ext.hpp>
#include <map>
#include <vector>
#include <stdbool.h>

#include <Resources/Material.hpp>
#include <Camera/Camera.hpp>
#include <Water/Water.hpp>

Material::Material() {}

Material::Material(GLuint* program) {
    this->program = program;
}

Material::~Material() {}

Material* Material::use() {
    glUseProgram(*this->program);
    this->loadToProgram(this->program);
    return this;
}

Material* Material::extend(Material* material) {
    this->program = material->program;

    this->albedoTexture = material->albedoTexture;
    this->normalTexture = material->normalTexture;
    this->aoTexture = material->aoTexture;
    this->roughnessTexture = material->roughnessTexture;
    this->metallicTexture = material->metallicTexture;

    this->modelMatrixLocation = material->modelMatrixLocation;
    this->modelViewProjectionMatrixLocation = material->modelViewProjectionMatrixLocation;
    this->jointTransformsLocation = material->jointTransformsLocation;


    this->params = material->params;
    this->paramsFunctions = material->paramsFunctions;

    return this;
}

Material* Material::setProgram(GLuint* program) {
    this->program = program;
    return this;
}

GLuint* Material::getAlbedo() {
    return &this->albedoTexture;
}

GLuint* Material::getNormal() {
    return &this->normalTexture;
}

GLuint* Material::getAO() {
    return &this->aoTexture;
}

GLuint* Material::getRoughness() {
    return &this->roughnessTexture;
}

GLuint* Material::getMetallic() {
    return &this->metallicTexture;
}

Material* Material::setAlbedo(GLint* resourceLocation, GLuint* diffuseTexture) {
    this->albedoTexture = *diffuseTexture;
    this->setTexture(ALBEDO_TEXTURE, resourceLocation, diffuseTexture);
    return this;
}

Material* Material::setNormal(GLint *resourceLocation, GLuint* normalTexture) {
    this->normalTexture = *normalTexture;
    this->setTexture(NORMAL_TEXTURE, resourceLocation, normalTexture);
    return this;
}

Material* Material::setAO(GLint *resourceLocation, GLuint* aoTexture) {
    this->aoTexture = *aoTexture;
    this->setTexture(AO_TEXTURE, resourceLocation, aoTexture);
    return this;
}

Material* Material::setRoughness(GLint *resourceLocation, GLuint* roughnessTexture) {
    this->roughnessTexture = *roughnessTexture;
    this->setTexture(ROUGHNESS_TEXTURE, resourceLocation, roughnessTexture);
    return this;
}

Material* Material::setMetallic(GLint *resourceLocation, GLuint* metallicTexture) {
    this->metallicTexture = *metallicTexture;
    this->setTexture(METALLIC_TEXTURE, resourceLocation, metallicTexture);
    return this;
}

Material* Material::setTexture(int textureType, GLint *resourceLocation, GLuint* texture) {
    this->paramsFunctions.push_back([=](GLuint program) -> void {
        glActiveTexture(GL_TEXTURE0 + textureType);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glUniform1i(*resourceLocation, textureType);
    });
    return this;
}

Material* Material::setParam(GLint *resourceLocation, glm::vec3 value) {
    this->paramsFunctions.push_back([=](GLuint program) -> void {
        glUniform3f(*resourceLocation, value.x, value.y, value.z);
    });
    return this;
}

Material* Material::setParam(GLint *resourceLocation, float value) {
    this->paramsFunctions.push_back([=](GLuint program) -> void {
        glUniform1f(*resourceLocation, value);
    });
    return this;
}

Material* Material::withCaustics(GLint *resourceLocation) {
    this->paramsFunctions.push_back([=](GLuint program) -> void {
        glUniform1i(*resourceLocation, 5);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, waterObject->getCausticsMap());
    });
    return this;
}

Material* Material::withCameraPosition(GLint *resourceLocation) {
    this->paramsFunctions.push_back([=](GLuint program) -> void {
        glm::vec3 cameraPos = camera->getPosition();
        glUniform3f(*resourceLocation, cameraPos.x, cameraPos.y, cameraPos.z);
    });
    return this;
}

Material* Material::withLightPosition(GLint *resourceLocation) {
    glm::vec3 lightDir = glm::normalize(glm::vec3(-92.0f, 222.0f, -256.0f));
    glm::vec3 lightPos = lightDir * 100000000.0f;
    this->setParam(resourceLocation, lightPos);
    return this;
}

Material* Material::withWaterHeight(GLint *resourceLocation) {
    this->setParam(resourceLocation, 192.0f);
    return this;
}

Material* Material::withJointTransforms(GLint *resourceLocation) {
    this->jointTransformsLocation = resourceLocation;
    return this;
}

Material* Material::withLightTransformation(GLint *resourceLocation) {
    this->paramsFunctions.push_back([=](GLuint program) -> void {
        glUniformMatrix4fv(*resourceLocation, 1, GL_FALSE, glm::value_ptr(waterObject->getLightCamera()));
    });
    return this;
}

Material* Material::withModelMatrix(GLint *resourceLocation) {
    this->modelMatrixLocation = resourceLocation;
    return this;
}

Material* Material::withModelViewProjectionMatrix(GLint *resourceLocation) {
    this->modelViewProjectionMatrixLocation = resourceLocation;
    return this;
}

Material* Material::setModelMatrix(glm::mat4 modelMatrix) {
    glUniformMatrix4fv(*this->modelMatrixLocation, 1, GL_FALSE, (float*)&(modelMatrix));
    return this;
}

Material* Material::setModelViewProjectionMatrix(glm::mat4 modelViewProjectionMatrix) {
    glUniformMatrix4fv(*this->modelViewProjectionMatrixLocation, 1, GL_FALSE, (float*)&(modelViewProjectionMatrix));
    return this;
}

Material* Material::setJointTransforms(std::vector<glm::mat4> jointTransforms) {
    glUniformMatrix4fv(*this->jointTransformsLocation, 20, GL_FALSE, glm::value_ptr(jointTransforms[0]));
    return this;
}

Material* Material::loadToProgram(GLuint* program) {
    for (const auto& func : this->paramsFunctions) {
        func(*program);
    }
    return this;
}

bool Material::hasParam(std::string name) {
    return std::find(this->params.begin(), this->params.end(), name) != this->params.end();
}