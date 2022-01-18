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
    static char *readFile(const char *name, size_t *size);
    static void saveFile(const char *file, const char *data, size_t size);

    int totalResources = 0;
    int loadedResources = 0;

    // textures
    GLuint tex_grid = 0;
    GLuint tex_gridColor = 0;
    GLuint tex_earth = 0;
    GLuint tex_earth2 = 0;
    GLuint tex_moon = 0;
    GLuint tex_ship = 0;
    GLuint tex_earthNormal = 0;
    GLuint tex_asteroidNormal = 0;
    GLuint tex_shipNormal = 0;
    GLuint tex_asteroid = 0;
    GLuint tex_wall = 0;
    GLuint tex_wallNormal = 0;
    GLuint tex_wallHeight = 0;
    GLuint tex_skybox = 0;
    GLuint tex_dummy = 0;
    GLuint tex_uv = 0;

    // programs:
    GLuint p_simple_color_shader = 0;
    bool p_simple_color_shader_loaded = false;
    GLint p_simple_color_shader_attr_vertexColor = -1;
    GLint p_simple_color_shader_attr_vertexPosition = -1;
    GLint p_simple_color_shader_uni_transformation = -1;
    
    GLuint p_simple_tex_shader = 0;
    bool p_simple_tex_shader_loaded = false;
    GLint p_simple_tex_shader_attr_vertexPosition = -1;
    GLint p_simple_tex_shader_attr_vertexTex = -1;
    GLint p_simple_tex_shader_uni_textureSampler = -1;
    GLint p_simple_tex_shader_uni_transformation = -1;

    GLuint p_skybox_shader = 0;
    bool p_skybox_shader_loaded = false;
    GLint p_skybox_shader_attr_aPos = -1;
    GLint p_skybox_shader_uni_projection = -1;
    GLint p_skybox_shader_uni_skybox = -1;

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

    // WATER SURFACE
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
    GLint p_simplex_attr_pos = -1;
    GLint p_simplex_attr_tex = -1;
    GLint p_simplex_uni_p = -1;
    GLint p_simplex_uni_scale = -1;
    GLint p_simplex_uni_translation = -1;
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
    GLint p_caustics_uni_height = -1;
    GLint p_caustics_uni_lightDirection = -1;
    GLint p_caustics_uni_modelMatrix = -1;
    GLint p_caustics_uni_transformation = -1;
    GLint p_caustics_uni_environmentMap = -1;
    GLint p_caustics_uni_heightMap = -1;
    GLint p_caustics_uni_normalMap = -1;
    // caustics_shader:
    GLuint p_caustics_shader = 0;
    bool p_caustics_shader_loaded = false;
    GLint p_caustics_shader_attr_vertexBitangent = -1;
    GLint p_caustics_shader_attr_vertexNormal = -1;
    GLint p_caustics_shader_attr_vertexPosition = -1;
    GLint p_caustics_shader_attr_vertexTangent = -1;
    GLint p_caustics_shader_attr_vertexTexCoord = -1;
    GLint p_caustics_shader_uni_cameraPosition = -1;
    GLint p_caustics_shader_uni_lightDirection = -1;
    GLint p_caustics_shader_uni_lightPosition = -1;
    GLint p_caustics_shader_uni_lightTransformation = -1;
    GLint p_caustics_shader_uni_modelMatrix = -1;
    GLint p_caustics_shader_uni_transformation = -1;
    GLint p_caustics_shader_uni_caustics = -1;
    GLint p_caustics_shader_uni_colorTexture = -1;
    GLint p_caustics_shader_uni_normalSampler = -1;

    static void loadModelExternal(const char *name, Model *out);

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
    GLuint compileShader(GLenum shaderType, const char *name);
    GLuint createProgram(const char *name, int shadersCount, GLuint *shaders);
};

extern ResourceLoader* resourceLoaderExternal;