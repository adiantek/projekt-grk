#pragma once

#include <opengl.h>
#include <stdbool.h>

class ResourceLoader
{
public:
    ResourceLoader();
    ~ResourceLoader();
    /**
     * return true if everything loaded
     */
    bool loadResources();
    void dumpProgram(const char *name, GLuint program);

    int totalResources = 0;
    int loadedResources = 0;

    // textures
    GLuint txt_grid = 0;
    GLuint txt_gridColor = 0;
    GLuint txt_earth = 0;
    GLuint txt_earth2 = 0;
    GLuint txt_moon = 0;
    GLuint txt_ship = 0;
    GLuint txt_earthNormal = 0;
    GLuint txt_asteroidNormal = 0;
    GLuint txt_shipNormal = 0;
    GLuint txt_asteroid = 0;
    GLuint txt_wall = 0;
    GLuint txt_wallNormal = 0;
    GLuint txt_wallHeight = 0;

    // programs
    GLuint p_shader_4_1 = 0;
    bool p_shader_4_1_loaded = false;
    GLint p_shader_4_1_attr_vertexNormal = -1;
    GLint p_shader_4_1_attr_vertexPosition = -1;
    GLint p_shader_4_1_uni_cameraPos = -1;
    GLint p_shader_4_1_uni_lightPos = -1;
    GLint p_shader_4_1_uni_modelMatrix = -1;
    GLint p_shader_4_1_uni_objectColor = -1;
    GLint p_shader_4_1_uni_transformation = -1;

    GLuint p_shader_4_sun = 0;
    bool p_shader_4_sun_loaded = false;
    GLint p_shader_4_sun_attr_vertexNormal = -1;
    GLint p_shader_4_sun_attr_vertexPosition = -1;
    GLint p_shader_4_sun_uni_cameraPos = -1;
    GLint p_shader_4_sun_uni_modelMatrix = -1;
    GLint p_shader_4_sun_uni_modelViewProjectionMatrix = -1;
    GLint p_shader_4_sun_uni_objectColor = -1;

    GLuint p_shader_4_tex = 0;
    bool p_shader_4_tex_loaded = false;
    GLint p_shader_4_tex_attr_vertexBitangent = -1;
    GLint p_shader_4_tex_attr_vertexNormal = -1;
    GLint p_shader_4_tex_attr_vertexPosition = -1;
    GLint p_shader_4_tex_attr_vertexTangent = -1;
    GLint p_shader_4_tex_attr_vertexTexCoord = -1;
    GLint p_shader_4_tex_uni_cameraPos = -1;
    GLint p_shader_4_tex_uni_colorTexture = -1;
    GLint p_shader_4_tex_uni_lightPos = -1;
    GLint p_shader_4_tex_uni_modelMatrix = -1;
    GLint p_shader_4_tex_uni_normalSampler = -1;
    GLint p_shader_4_tex_uni_transformation = -1;
    
    GLuint p_shader_4_tex_with_parallax = 0;
    bool p_shader_4_tex_with_parallax_loaded = false;
    GLint p_shader_4_tex_with_parallax_attr_vertexBitangent = -1;
    GLint p_shader_4_tex_with_parallax_attr_vertexNormal = -1;
    GLint p_shader_4_tex_with_parallax_attr_vertexPosition = -1;
    GLint p_shader_4_tex_with_parallax_attr_vertexTangent = -1;
    GLint p_shader_4_tex_with_parallax_attr_vertexTexCoord = -1;
    GLint p_shader_4_tex_with_parallax_uni_cameraPos = -1;
    GLint p_shader_4_tex_with_parallax_uni_colorTexture = -1;
    GLint p_shader_4_tex_with_parallax_uni_lightPos = -1;
    GLint p_shader_4_tex_with_parallax_uni_modelMatrix = -1;
    GLint p_shader_4_tex_with_parallax_uni_normalSampler = -1;
    GLint p_shader_4_tex_with_parallax_uni_heightSampler = -1;
    GLint p_shader_4_tex_with_parallax_uni_transformation = -1;
    GLint p_shader_4_tex_with_parallax_uni_heightScale = -1;

    GLuint p_shader_color = 0;
    bool p_shader_color_loaded = false;
    GLint p_shader_color_attr_vertexNormal = -1;
    GLint p_shader_color_attr_vertexPosition = -1;
    GLint p_shader_color_uni_lightDir = -1;
    GLint p_shader_color_uni_modelMatrix = -1;
    GLint p_shader_color_uni_modelViewProjectionMatrix = -1;
    GLint p_shader_color_uni_objectColor = -1;

    GLuint p_shader_tex = 0;
    bool p_shader_tex_loaded = false;
    GLint p_shader_tex_attr_vertexNormal = -1;
    GLint p_shader_tex_attr_vertexPosition = -1;
    GLint p_shader_tex_attr_vertexTexCoord = -1;
    GLint p_shader_tex_uni_lightDir = -1;
    GLint p_shader_tex_uni_modelMatrix = -1;
    GLint p_shader_tex_uni_modelViewProjectionMatrix = -1;
    GLint p_shader_tex_uni_textureSampler = -1;

private:
    int totalResourcesCounter = 0;
    bool all_loaded = false;
    double startFrame = 0;
    GLuint* shaders = 0;
    size_t shadersCap = 0;
    size_t shadersCompiled = 0;
    bool canLoadNextResource();
    void loadTextures();
    void loadPrograms();
    void loadAttributesAndUniforms();
    void loadTexture(const char *name, GLuint *out);
    /**
     * true if and only if program was loaded right now
     */
    bool loadProgram(const char *name, GLuint *out_program, bool *out_loaded, int shadersCount, ...);
    char *readFile(const char *name, size_t *size);
    GLuint compileShader(GLenum shaderType, const char *name);
    GLuint createProgram(const char *name, int shadersCount, GLuint *shaders);
};