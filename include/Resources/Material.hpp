#pragma once

#include <vector>
#include <functional>
#include <map>
#include <stdbool.h>
#include <opengl.h>

class Material {
    public:
      inline static const int ALBEDO_TEXTURE = 0;
      inline static const int NORMAL_TEXTURE = 1;
      inline static const int AO_TEXTURE = 2;
      inline static const int ROUGHNESS_TEXTURE = 3;
      inline static const int METALLIC_TEXTURE = 4;

      Material();
      Material(GLuint* program);
      Material(GLuint* program, std::map<int, GLuint> textures);
      ~Material();

      Material* extend(Material* material);

      Material* setProgram(GLuint* program);

      Material* setAlbedo(GLint* resourceLocation, GLuint* diffuseTexture);
      Material* setNormal(GLint *resourceLocation, GLuint* normalTexture);
      Material* setAO(GLint *resourceLocation, GLuint* aoTexture);
      Material* setRoughness(GLint *resourceLocation, GLuint* roughnessTexture);
      Material* setMetallic(GLint *resourceLocation, GLuint* metallicTexture);
      Material* setTexture (int textureType, GLint *resourceLocation, GLuint* texture);

      Material* setParam(GLint *resourceLocation, glm::vec3 value);
      Material* setParam(GLint *resourceLocation, float value);

      Material* withCaustics(GLint *resourceLocation);
      Material* withCameraPosition(GLint *resourceLocation);
      Material* withLightPosition(GLint *resourceLocation);
      Material* withWaterHeight(GLint *resourceLocation);
      Material* withJointTransforms(GLint *resourceLocation);
      Material* withLightTransformation(GLint *resourceLocation);
      Material* withModelMatrix(GLint *resourceLocation);
      Material* withModelViewProjectionMatrix(GLint *resourceLocation);

      Material* setModelMatrix(glm::mat4 modelMatrix);
      Material* setModelViewProjectionMatrix(glm::mat4 modelViewProjectionMatrix);
      Material* setJointTransforms(std::vector<glm::mat4> jointTransforms);

      bool hasParam(std::string name);

      GLuint* getAlbedo();
      GLuint* getNormal();
      GLuint* getAO();
      GLuint* getRoughness();
      GLuint* getMetallic();

      GLuint* program;

      Material *loadToProgram(GLuint* program);
      Material* use();

    private:
      GLuint albedoTexture = -1;
      GLuint normalTexture = -1;
      GLuint aoTexture = -1;
      GLuint roughnessTexture = -1;
      GLuint metallicTexture = -1;

      GLint* modelMatrixLocation;
      GLint* modelViewProjectionMatrixLocation;
      GLint* jointTransformsLocation;

      std::vector<std::function<void(GLuint)>> paramsFunctions = std::vector<std::function<void(GLuint)>>();
      std::vector<std::string> params = std::vector<std::string>();

};