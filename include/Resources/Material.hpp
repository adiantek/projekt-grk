#pragma once

#include <vector>
#include <functional>
#include <map>
#include <stdbool.h>
#include <opengl.h>

class Material {
    public:
      inline static const int DIFFUSE_TEXTURE = 0;
      inline static const int NORMAL_TEXTURE = 1;
      inline static const int AO_TEXTURE = 2;
      inline static const int ROUGHNESS_TEXTURE = 3;

      Material::Material();
      Material::Material(GLuint* program);
      Material::Material(GLuint* program, std::map<int, GLuint> textures);
      Material::~Material();

      Material* extend(Material* material);

      Material* setProgram(GLuint* program);

      Material* setDiffuse(GLuint* diffuseTexture);
      Material* setNormal(GLuint* normalTexture);
      Material* setAO(GLuint* aoTexture);
      Material* setRoughness(GLuint* roughnessTexture);

      Material* setParam(std::string name, glm::vec3 value);
      Material* setParam(std::string name, float value);
      Material* setParam(std::string name, glm::mat4 value);
      Material* setParam(std::string name);

      bool hasParam(std::string name);

      GLuint* getDiffuse();
      GLuint* getNormal();
      GLuint* getAO();
      GLuint* getRoughness();

      GLuint* program;

      void loadToProgram(GLuint* program);

    private:
      GLuint diffuseTexture;
      GLuint normalTexture;
      GLuint aoTexture;
      GLuint roughnessTexture;

      std::vector<std::function<void(GLuint)>> paramsFunctions = std::vector<std::function<void(GLuint)>>();
      std::vector<std::string> params = std::vector<std::string>();
};