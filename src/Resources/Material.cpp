#include <opengl.h>
#include <glm/ext.hpp>

#include <Resources/Material.hpp>
#include <map>
#include <vector>
#include <stdbool.h>

Material::Material() {}

Material::Material(GLuint* program, std::map<int, GLuint> textures) {
    this->program = program;

    for (const auto& [key, value] : textures) {
        switch (key) {
            case Material::DIFFUSE_TEXTURE:
                this->diffuseTexture = value;
                break;
            case Material::NORMAL_TEXTURE:
                this->normalTexture = value;
                break;
            case Material::AO_TEXTURE:
                this->aoTexture = value;
                break;
            case Material::ROUGHNESS_TEXTURE:
                this->roughnessTexture = value;
                break;
        }
    }
}

Material::Material(GLuint* program) {
    this->program = program;
}

Material::~Material() {}

Material* Material::extend(Material* material) {
    this->program = material->program;

    this->diffuseTexture = material->diffuseTexture;
    this->normalTexture = material->normalTexture;
    this->aoTexture = material->aoTexture;
    this->roughnessTexture = material->roughnessTexture;

    return this;
}

Material* Material::setProgram(GLuint* program) {
    this->program = program;
    return this;
}

GLuint* Material::getDiffuse() {
    return &this->diffuseTexture;
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

Material* Material::setDiffuse(GLuint* diffuseTexture) {
    this->diffuseTexture = *diffuseTexture;
    return this;
}

Material* Material::setNormal(GLuint* normalTexture) {
    this->normalTexture = *normalTexture;
    return this;
}

Material* Material::setAO(GLuint* aoTexture) {
    this->aoTexture = *aoTexture;
    return this;
}

Material* Material::setRoughness(GLuint* roughnessTexture) {
    this->roughnessTexture = *roughnessTexture;
    return this;
}

Material* Material::setParam(std::string name, glm::vec3 value) {
    this->paramsFunctions.push_back([=](GLuint program) -> void {
        glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z);
    });
    this->params.push_back(name);
    return this;
}

Material* Material::setParam(std::string name, glm::mat4 value) {
    this->paramsFunctions.push_back([=](GLuint program) -> void {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, (float*)&value);
    });
    this->params.push_back(name);
    return this;
}

Material* Material::setParam(std::string name) {
    this->params.push_back(name);
    return this;
}

void Material::loadToProgram(GLuint* program) {
    for (const auto& func : this->paramsFunctions) {
        func(*program);
    }
}

bool Material::hasParam(std::string name) {
    return std::find(this->params.begin(), this->params.end(), name) != this->params.end();
}