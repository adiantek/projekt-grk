#pragma once

#include <opengl.h>
#include <stdbool.h>

#include <Resources/Model.hpp>

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
    GLuint txt_skybox = 0;
    GLuint txt_dummy = 0;

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
    
    // shader_4_tex_with_parallax:
    GLuint p_shader_4_tex_with_parallax = 0;
    bool p_shader_4_tex_with_parallax_loaded = false;
    GLint p_shader_4_tex_with_parallax_attr_vertexBitangent = -1;
    GLint p_shader_4_tex_with_parallax_attr_vertexNormal = -1;
    GLint p_shader_4_tex_with_parallax_attr_vertexPosition = -1;
    GLint p_shader_4_tex_with_parallax_attr_vertexTangent = -1;
    GLint p_shader_4_tex_with_parallax_attr_vertexTexCoord = -1;
    GLint p_shader_4_tex_with_parallax_uni_cameraPos = -1;
    GLint p_shader_4_tex_with_parallax_uni_colorTexture = -1;
    GLint p_shader_4_tex_with_parallax_uni_depthSampler = -1;
    GLint p_shader_4_tex_with_parallax_uni_heightScale = -1;
    GLint p_shader_4_tex_with_parallax_uni_lightPos = -1;
    GLint p_shader_4_tex_with_parallax_uni_modelMatrix = -1;
    GLint p_shader_4_tex_with_parallax_uni_normalSampler = -1;
    GLint p_shader_4_tex_with_parallax_uni_transformation = -1;

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

    GLuint p_water_simulation = 0;
    bool p_water_simulation_loaded = false;
    GLint p_water_simulation_attr_vertexPosition = -1;
    GLint p_water_simulation_uni_scale = -1;
    GLint p_water_simulation_uni_time = -1;
    GLint p_water_simulation_uni_transition = -1;
    GLint p_water_simulation_uni_waveCount = -1;

    GLuint p_water_surface = 0;
    bool p_water_surface_loaded = false;
    GLint p_water_surface_attr_position = -1;
    GLint p_water_surface_attr_texturePosition = -1;
    GLint p_water_surface_uni_cameraPosition = -1;
    GLint p_water_surface_uni_heightMap = -1;
    GLint p_water_surface_uni_model = -1;
    GLint p_water_surface_uni_normalMap = -1;
    GLint p_water_surface_uni_skybox = -1;
    GLint p_water_surface_uni_transformation = -1;
    // simplex:
    GLuint p_simplex = 0;
    bool p_simplex_loaded = false;
    GLint p_simplex_attr_vertexPosition = -1;
    GLint p_simplex_uni_alpha = -1;
    GLint p_simplex_uni_p = -1;
    GLint p_simplex_uni_scale = -1;
    // environment_map:
    GLuint p_environment_map = 0;
    bool p_environment_map_loaded = false;
    GLint p_environment_map_attr_vertexPosition = -1;
    GLint p_environment_map_uni_modelMatrix = -1;
    GLint p_environment_map_uni_transformation = -1;
    // caustics:
    GLuint p_caustics = 0;
    bool p_caustics_loaded = false;
    GLint p_caustics_attr_texturePosition = -1;
    GLint p_caustics_attr_vertexPosition = -1;
    GLint p_caustics_uni_deltaEnvTexture = -1;
    GLint p_caustics_uni_depthMap = -1;
    GLint p_caustics_uni_environmentMap = -1;
    GLint p_caustics_uni_light = -1;
    GLint p_caustics_uni_normalMap = -1;
    GLint p_caustics_uni_transformation = -1;
    // caustics_env:
    GLuint p_caustics_env = 0;
    bool p_caustics_env_loaded = false;
    GLint p_caustics_env_attr_vertexBitangent = -1;
    GLint p_caustics_env_attr_vertexNormal = -1;
    GLint p_caustics_env_attr_vertexPosition = -1;
    GLint p_caustics_env_attr_vertexTangent = -1;
    GLint p_caustics_env_attr_vertexTexCoord = -1;
    GLint p_caustics_env_uni_cameraPos = -1;
    GLint p_caustics_env_uni_lightPos = -1;
    GLint p_caustics_env_uni_modelMatrix = -1;
    GLint p_caustics_env_uni_transformation = -1;

    static void loadTextureExternal(char *name, GLuint *out);
    static void loadModelExternal(char *name, Model *out);

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
    void loadTextureCubeMap(GLuint *out);
    /**
     * true if and only if program was loaded right now
     */
    bool loadProgram(const char *name, GLuint *out_program, bool *out_loaded, int shadersCount, ...);
    char *readFile(const char *name, size_t *size);
    GLuint compileShader(GLenum shaderType, const char *name);
    GLuint createProgram(const char *name, int shadersCount, GLuint *shaders);
};

extern ResourceLoader* resourceLoaderExternal;