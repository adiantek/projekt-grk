#pragma once

#include <opengl.h>
#include <stdbool.h>
#include <Sound.hpp>

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
    GLuint tex_skybox = 0;
    GLuint tex_dummy = 0;
    GLuint tex_bubbleTexture = 0;
    GLuint tex_entities_barracuda_barracuda_albedo = 0;
    GLuint tex_entities_barracuda_barracuda_normal = 0;
    GLuint tex_entities_pilotfish_pilotfish_albedo = 0;
    GLuint tex_entities_red_snapper_red_snapper_albedo = 0;
    GLuint tex_entities_red_snapper_red_snapper_normal = 0;
    GLuint tex_entities_robot_robot_body_AO = 0;
    GLuint tex_entities_robot_robot_body_Albedo = 0;
    GLuint tex_entities_robot_robot_body_Metalness = 0;
    GLuint tex_entities_robot_robot_body_Normal = 0;
    GLuint tex_entities_robot_robot_body_Roughness = 0;
    GLuint tex_entities_robot_robot_chrome_AO = 0;
    GLuint tex_entities_robot_robot_chrome_Albedo = 0;
    GLuint tex_entities_robot_robot_chrome_Metalness = 0;
    GLuint tex_entities_robot_robot_chrome_Normal = 0;
    GLuint tex_entities_robot_robot_chrome_Roughness = 0;
    GLuint tex_entities_robot_robot_glow_AO = 0;
    GLuint tex_entities_robot_robot_glow_Albedo = 0;
    GLuint tex_entities_robot_robot_glow_Metalness = 0;
    GLuint tex_entities_robot_robot_glow_Normal = 0;
    GLuint tex_entities_robot_robot_glow_Roughness = 0;
    GLuint tex_entities_robot_robot_lens_AO = 0;
    GLuint tex_entities_robot_robot_lens_Albedo = 0;
    GLuint tex_entities_robot_robot_lens_Metalness = 0;
    GLuint tex_entities_robot_robot_lens_Normal = 0;
    GLuint tex_entities_robot_robot_lens_Roughness = 0;
    GLuint tex_entities_robot_robot_metal_2_AO = 0;
    GLuint tex_entities_robot_robot_metal_2_Albedo = 0;
    GLuint tex_entities_robot_robot_metal_2_Metalness = 0;
    GLuint tex_entities_robot_robot_metal_2_Normal = 0;
    GLuint tex_entities_robot_robot_metal_2_Roughness = 0;
    GLuint tex_entities_robot_robot_metal_AO = 0;
    GLuint tex_entities_robot_robot_metal_Albedo = 0;
    GLuint tex_entities_robot_robot_metal_Metalness = 0;
    GLuint tex_entities_robot_robot_metal_Normal = 0;
    GLuint tex_entities_robot_robot_metal_Roughness = 0;
    GLuint tex_entities_robot_robot_propelers_AO = 0;
    GLuint tex_entities_robot_robot_propelers_Albedo = 0;
    GLuint tex_entities_robot_robot_propelers_Metalness = 0;
    GLuint tex_entities_robot_robot_propelers_Normal = 0;
    GLuint tex_entities_robot_robot_propelers_Roughness = 0;
    GLuint tex_entities_robot_robot_wire_AO = 0;
    GLuint tex_entities_robot_robot_wire_Albedo = 0;
    GLuint tex_entities_robot_robot_wire_Metalness = 0;
    GLuint tex_entities_robot_robot_wire_Normal = 0;
    GLuint tex_entities_robot_robot_wire_Roughness = 0;
    GLuint tex_entities_wild_golden_fish_wild_golden_fish_albedo = 0;
    GLuint tex_entities_wild_golden_fish_wild_golden_fish_normal = 0;
    GLuint tex_foliage_kelp_kelp_albedo = 0;
    GLuint tex_foliage_kelp_kelp_ao = 0;
    GLuint tex_foliage_kelp_kelp_normal = 0;
    GLuint tex_foliage_seagrass_dried_grass_albedo = 0;
    GLuint tex_foliage_seagrass_dried_grass_height = 0;
    GLuint tex_foliage_seagrass_dried_grass_normal = 0;
    GLuint tex_foliage_seagrass_grass_blades_albedo = 0;
    GLuint tex_foliage_seagrass_grass_blades_ao = 0;
    GLuint tex_foliage_seagrass_grass_blades_normal = 0;
    GLuint tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_albedo = 0;
    GLuint tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_ao = 0;
    GLuint tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_height = 0;
    GLuint tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_normal = 0;
    GLuint tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_roughness = 0;
    GLuint tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_albedo = 0;
    GLuint tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_ao = 0;
    GLuint tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_height = 0;
    GLuint tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_normal = 0;
    GLuint tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_roughness = 0;
    GLuint tex_generic_Nature_SandBeach1_2x2_1K_Nature_SandBeach_1K_albedo = 0;
    GLuint tex_generic_Nature_SandBeach1_2x2_1K_Nature_SandBeach_1K_height = 0;
    GLuint tex_generic_Nature_SandBeach1_2x2_1K_Nature_SandBeach_1K_normal = 0;
    GLuint tex_generic_Nature_SandBeach1_2x2_1K_Nature_SandBeach_1K_roughness = 0;
    GLuint tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_albedo = 0;
    GLuint tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_ao = 0;
    GLuint tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_height = 0;
    GLuint tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_normal = 0;
    GLuint tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_roughness = 0;
    GLuint tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_albedo = 0;
    GLuint tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_ao = 0;
    GLuint tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_height = 0;
    GLuint tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_normal = 0;
    GLuint tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_roughness = 0;
    GLuint tex_grid = 0;
    GLuint tex_grid_color = 0;
    GLuint tex_props_chest_chest_reinforcment_albedo = 0;
    GLuint tex_props_chest_chest_reinforcment_ao = 0;
    GLuint tex_props_chest_chest_reinforcment_metallic = 0;
    GLuint tex_props_chest_chest_reinforcment_normal = 0;
    GLuint tex_props_chest_chest_reinforcment_roughness = 0;
    GLuint tex_props_chest_chest_wood_albedo = 0;
    GLuint tex_props_chest_chest_wood_ao = 0;
    GLuint tex_props_chest_chest_wood_metallic = 0;
    GLuint tex_props_chest_chest_wood_normal = 0;
    GLuint tex_props_chest_chest_wood_roughness = 0;
    GLuint tex_props_coin_coin_albedo = 0;
    GLuint tex_props_coin_coin_ao = 0;
    GLuint tex_props_coin_coin_height = 0;
    GLuint tex_props_coin_coin_metallic = 0;
    GLuint tex_props_coin_coin_normal = 0;
    GLuint tex_props_coin_coin_roughness = 0;
    GLuint tex_rocks_rock_big1_rock_big1_albedo = 0;
    GLuint tex_rocks_rock_big1_rock_big1_ao = 0;
    GLuint tex_rocks_rock_big1_rock_big1_height = 0;
    GLuint tex_rocks_rock_big1_rock_big1_normal = 0;
    GLuint tex_rocks_rock_small1_rock_small1_albedo = 0;
    GLuint tex_rocks_rock_small1_rock_small1_ao = 0;
    GLuint tex_rocks_rock_small1_rock_small1_height = 0;
    GLuint tex_rocks_rock_small1_rock_small1_normal = 0;
    GLuint tex_rocks_rock_small2_rock_small2_albedo = 0;
    GLuint tex_rocks_rock_small2_rock_small2_ao = 0;
    GLuint tex_rocks_rock_small2_rock_small2_height = 0;
    GLuint tex_rocks_rock_small2_rock_small2_normal = 0;
    GLuint tex_skybox_back = 0;
    GLuint tex_skybox_bottom = 0;
    GLuint tex_skybox_front = 0;
    GLuint tex_skybox_left = 0;
    GLuint tex_skybox_right = 0;
    GLuint tex_skybox_top = 0;
    GLuint tex_test = 0;
    GLuint tex_test_normals = 0;
    GLuint tex_uv_grid = 0;
    GLuint tex_wall = 0;
    GLuint tex_wall_ao = 0;
    GLuint tex_wall_height = 0;
    GLuint tex_wall_normal = 0;
    GLuint tex_wall_roughness = 0;

    // models:
    Model *m_entities_barracuda = 0;
    Model *m_entities_pilotfish = 0;
    Model *m_entities_red_snapper = 0;
    Model *m_entities_wild_golden_fish = 0;
    Model *m_foliage_kelp = 0;
    Model *m_foliage_seagrass = 0;
    Model *m_primitives_complex_cube = 0;
    Model *m_primitives_cube = 0;
    Model *m_primitives_cylinder = 0;
    Model *m_primitives_plane = 0;
    Model *m_primitives_sphere = 0;
    Model *m_primitives_torus = 0;
    Model *m_props_chest = 0;
    Model *m_props_coin = 0;
    Model *m_robot = 0;
    Model *m_robot_with_armature = 0;
    Model *m_rocks_rock_big1 = 0;
    Model *m_rocks_rock_small1 = 0;
    Model *m_rocks_rock_small2 = 0;
    Model *m_spaceship = 0;
    Model *m_sphere = 0;
    Model *m_sphere2 = 0;
    Model *m_sphere_different_texcoords = 0;

    // sounds:
    ALuint sound_chestopen = 0xFFFFFFFF;
    ALuint sound_chestclosed = 0xFFFFFFFF;
    ALuint sound_bg = 0xFFFFFFFF;
    ALuint sound_coin = 0xFFFFFFFF;

    // programs:
    GLuint p_color_armature = 0;
    bool p_color_armature_loaded = false;
    GLint p_color_armature_attr_vertexBitangent = -1;
    GLint p_color_armature_attr_vertexJoints = -1;
    GLint p_color_armature_attr_vertexNormal = -1;
    GLint p_color_armature_attr_vertexPosition = -1;
    GLint p_color_armature_attr_vertexTangent = -1;
    GLint p_color_armature_attr_vertexTexCoord = -1;
    GLint p_color_armature_attr_vertexWeights = -1;
    GLint p_color_armature_uni_albedoTexture = -1;
    GLint p_color_armature_uni_aoTexture = -1;
    GLint p_color_armature_uni_cameraPosition = -1;
    GLint p_color_armature_uni_caustics = -1;
    GLint p_color_armature_uni_jointTransforms = -1;
    GLint p_color_armature_uni_lightPosition = -1;
    GLint p_color_armature_uni_lightTransformation = -1;
    GLint p_color_armature_uni_metallicTexture = -1;
    GLint p_color_armature_uni_modelMatrix = -1;
    GLint p_color_armature_uni_modelViewProjectionMatrix = -1;
    GLint p_color_armature_uni_normalTexture = -1;
    GLint p_color_armature_uni_roughnessTexture = -1;
    GLint p_color_armature_uni_waterHeight = -1;
    // armature_shadow:
    GLuint p_armature_shadow = 0;
    bool p_armature_shadow_loaded = false;
    GLint p_armature_shadow_attr_vertexJoints = -1;
    GLint p_armature_shadow_attr_vertexPosition = -1;
    GLint p_armature_shadow_attr_vertexWeights = -1;
    GLint p_armature_shadow_uni_jointTransforms = -1;
    GLint p_armature_shadow_uni_modelMatrix = -1;
    GLint p_armature_shadow_uni_modelViewProjectionMatrix = -1;

    GLuint p_simple_color_shader = 0;
    bool p_simple_color_shader_loaded = false;
    GLint p_simple_color_shader_attr_vertexColor = -1;
    GLint p_simple_color_shader_attr_vertexPosition = -1;
    GLint p_simple_color_shader_uni_transformation = -1;

    GLuint p_simple_uni_color_shader = 0;
    bool p_simple_uni_color_shader_loaded = false;
    GLint p_simple_uni_color_shader_attr_vertexPosition = -1;
    GLint p_simple_uni_color_shader_uni_color = -1;
    GLint p_simple_uni_color_shader_uni_transformation = -1;
    
    GLuint p_simple_tex_shader = 0;
    bool p_simple_tex_shader_loaded = false;
    GLint p_simple_tex_shader_attr_vertexPosition = -1;
    GLint p_simple_tex_shader_attr_vertexTex = -1;
    GLint p_simple_tex_shader_uni_textureSampler = -1;
    GLint p_simple_tex_shader_uni_transformation = -1;

    // skybox
    GLuint p_skybox_shader = 0;
    bool p_skybox_shader_loaded = false;
    GLint p_skybox_shader_attr_aPos = -1;
    GLint p_skybox_shader_uni_projection = -1;
    GLint p_skybox_shader_uni_skybox = -1;

    // underwater_fog_shader:
    GLuint p_underwater_fog_shader = 0;
    bool p_underwater_fog_shader_loaded = false;
    GLint p_underwater_fog_shader_attr_aPos = -1;
    GLint p_underwater_fog_shader_attr_vertexTexCoord = -1;
    GLint p_underwater_fog_shader_uni_distanceToSurface = -1;
    GLint p_underwater_fog_shader_uni_depthTexture = -1;
    GLint p_underwater_fog_shader_uni_screenTexture = -1;

    // bubble_particle_shader:
    GLuint p_bubbles_shader = 0;
    bool p_bubbles_shader_loaded = false;
    GLint p_bubbles_shader_attr_vertexTexCoord = -1;
    GLint p_bubbles_shader_attr_vertexPosition = -1;
    GLint p_bubbles_shader_uni_distanceToSurface = -1;
    GLint p_bubbles_shader_uni_particlePositionsAndLife = -1;
    GLint p_bubbles_shader_uni_cameraMatrix = -1;
    GLint p_bubbles_shader_uni_cameraDirection = -1;
    GLint p_bubbles_shader_uni_cameraUp = -1;
    GLint p_bubbles_shader_uni_bubbleTexture = -1;
    
    GLuint p_shader_color = 0;
    bool p_shader_color_loaded = false;
    GLint p_shader_color_attr_vertexNormal = -1;
    GLint p_shader_color_attr_vertexPosition = -1;
    GLint p_shader_color_uni_lightDir = -1;
    GLint p_shader_color_uni_modelMatrix = -1;
    GLint p_shader_color_uni_modelViewProjectionMatrix = -1;
    GLint p_shader_color_uni_objectColor = -1;

    // WATER SURFACE
    GLuint p_water_simulation = 0;
    bool p_water_simulation_loaded = false;
    GLint p_water_simulation_attr_vertexPosition = -1;
    GLint p_water_simulation_uni_scale = -1;
    GLint p_water_simulation_uni_time = -1;
    GLint p_water_simulation_uni_transition = -1;
    GLint p_water_simulation_uni_waveCount = -1;
    GLint p_water_simulation_uni_waves = -1;

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
    GLint p_simplex_uni_weight = -1;
    GLint p_simplex_uni_translation = -1;
    GLint p_simplex_uni_layer = -1;
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
    // chunk:
    GLuint p_chunk = 0;
    bool p_chunk_loaded = false;
    GLint p_chunk_attr_vertexBitangent = -1;
    GLint p_chunk_attr_vertexNormal = -1;
    GLint p_chunk_attr_vertexPosition = -1;
    GLint p_chunk_attr_vertexTangent = -1;
    GLint p_chunk_attr_vertexTexCoord = -1;
    GLint p_chunk_uni_cameraPosition = -1;
    GLint p_chunk_uni_lightPosition = -1;
    GLint p_chunk_uni_lightTransformation = -1;
    GLint p_chunk_uni_modeSwitch = -1;
    GLint p_chunk_uni_modelMatrix = -1;
    GLint p_chunk_uni_transformation = -1;
    GLint p_chunk_uni_waterHeight = -1;
    GLint p_chunk_uni_aoMap = -1;
    GLint p_chunk_uni_caustics = -1;
    GLint p_chunk_uni_colorTexture = -1;
    GLint p_chunk_uni_depthMap = -1;
    GLint p_chunk_uni_normalSampler = -1;
    GLint p_chunk_uni_roughnessMap = -1;
    // coin:
    GLuint p_coin = 0;
    bool p_coin_loaded = false;
    GLint p_coin_attr_vertexBitangent = -1;
    GLint p_coin_attr_vertexNormal = -1;
    GLint p_coin_attr_vertexPosition = -1;
    GLint p_coin_attr_vertexTangent = -1;
    GLint p_coin_attr_vertexTexCoord = -1;
    GLint p_coin_uni_cameraPosition = -1;
    GLint p_coin_uni_lightPosition = -1;
    GLint p_coin_uni_lightTransformation = -1;
    GLint p_coin_uni_modelMatrix = -1;
    GLint p_coin_uni_transformation = -1;
    GLint p_coin_uni_waterHeight = -1;
    GLint p_coin_uni_aoMap = -1;
    GLint p_coin_uni_caustics = -1;
    GLint p_coin_uni_colorTexture = -1;
    GLint p_coin_uni_depthMap = -1;
    GLint p_coin_uni_metallicMap = -1;
    GLint p_coin_uni_normalSampler = -1;
    GLint p_coin_uni_roughnessMap = -1;
    // rock:
    GLuint p_rock = 0;
    bool p_rock_loaded = false;
    GLint p_rock_attr_vertexBitangent = -1;
    GLint p_rock_attr_vertexNormal = -1;
    GLint p_rock_attr_vertexPosition = -1;
    GLint p_rock_attr_vertexTangent = -1;
    GLint p_rock_attr_vertexTexCoord = -1;
    GLint p_rock_uni_cameraPosition = -1;
    GLint p_rock_uni_lightPosition = -1;
    GLint p_rock_uni_lightTransformation = -1;
    GLint p_rock_uni_modelMatrix = -1;
    GLint p_rock_uni_transformation = -1;
    GLint p_rock_uni_waterHeight = -1;
    GLint p_rock_uni_aoMap = -1;
    GLint p_rock_uni_caustics = -1;
    GLint p_rock_uni_colorTexture = -1;
    GLint p_rock_uni_depthMap = -1;
    GLint p_rock_uni_normalSampler = -1;
    // pilotfish:
    GLuint p_pilotfish = 0;
    bool p_pilotfish_loaded = false;
    GLint p_pilotfish_attr_vertexBitangent = -1;
    GLint p_pilotfish_attr_vertexNormal = -1;
    GLint p_pilotfish_attr_vertexPosition = -1;
    GLint p_pilotfish_attr_vertexTangent = -1;
    GLint p_pilotfish_attr_vertexTexCoord = -1;
    GLint p_pilotfish_uni_cameraPosition = -1;
    GLint p_pilotfish_uni_lightPosition = -1;
    GLint p_pilotfish_uni_lightTransformation = -1;
    GLint p_pilotfish_uni_modelMatrix = -1;
    GLint p_pilotfish_uni_transformation = -1;
    GLint p_pilotfish_uni_waterHeight = -1;
    GLint p_pilotfish_uni_caustics = -1;
    GLint p_pilotfish_uni_colorTexture = -1;
    GLint p_pilotfish_uni_time = -1;
    // cubefish:
    GLuint p_cubefish = 0;
    bool p_cubefish_loaded = false;
    GLint p_cubefish_attr_vertexBitangent = -1;
    GLint p_cubefish_attr_vertexNormal = -1;
    GLint p_cubefish_attr_vertexPosition = -1;
    GLint p_cubefish_attr_vertexTangent = -1;
    GLint p_cubefish_attr_vertexTexCoord = -1;
    GLint p_cubefish_uni_cameraPosition = -1;
    GLint p_cubefish_uni_lightPosition = -1;
    GLint p_cubefish_uni_lightTransformation = -1;
    GLint p_cubefish_uni_modelMatrix = -1;
    GLint p_cubefish_uni_transformation = -1;
    GLint p_cubefish_uni_waterHeight = -1;
    GLint p_cubefish_uni_aoMap = -1;
    GLint p_cubefish_uni_caustics = -1;
    GLint p_cubefish_uni_colorTexture = -1;
    GLint p_cubefish_uni_depthMap = -1;
    GLint p_cubefish_uni_normalSampler = -1;
    GLint p_cubefish_uni_roughnessMap = -1;
    // normal_fish:
    GLuint p_normal_fish = 0;
    bool p_normal_fish_loaded = false;
    GLint p_normal_fish_attr_vertexBitangent = -1;
    GLint p_normal_fish_attr_vertexNormal = -1;
    GLint p_normal_fish_attr_vertexPosition = -1;
    GLint p_normal_fish_attr_vertexTangent = -1;
    GLint p_normal_fish_attr_vertexTexCoord = -1;
    GLint p_normal_fish_uni_cameraPosition = -1;
    GLint p_normal_fish_uni_lightPosition = -1;
    GLint p_normal_fish_uni_lightTransformation = -1;
    GLint p_normal_fish_uni_modelMatrix = -1;
    GLint p_normal_fish_uni_time = -1;
    GLint p_normal_fish_uni_transformation = -1;
    GLint p_normal_fish_uni_waterHeight = -1;
    GLint p_normal_fish_uni_caustics = -1;
    GLint p_normal_fish_uni_colorTexture = -1;
    GLint p_normal_fish_uni_normalTexture = -1;
    // fish_shadow:
    GLuint p_fish_shadow = 0;
    bool p_fish_shadow_loaded = false;
    GLint p_fish_shadow_attr_vertexPosition = -1;
    GLint p_fish_shadow_uni_modelMatrix = -1;
    GLint p_fish_shadow_uni_time = -1;
    GLint p_fish_shadow_uni_transformation = -1;
    // glow:
    GLuint p_glow = 0;
    bool p_glow_loaded = false;
    GLint p_glow_attr_pos = -1;
    GLint p_glow_attr_tex = -1;
    GLint p_glow_uni_color = -1;
    GLint p_glow_uni_divider = -1;
    GLint p_glow_uni_maxSample = -1;
    GLint p_glow_uni_radius = -1;
    GLint p_glow_uni_glowTexture = -1;
    GLint p_glow_uni_texelSize = -1;
    // instanced_kelp:
    GLuint p_instanced_kelp = 0;
    bool p_instanced_kelp_loaded = false;
    GLint p_instanced_kelp_attr_vertexBitangent = -1;
    GLint p_instanced_kelp_attr_vertexNormal = -1;
    GLint p_instanced_kelp_attr_vertexPosition = -1;
    GLint p_instanced_kelp_attr_vertexTangent = -1;
    GLint p_instanced_kelp_attr_vertexTexCoord = -1;
    GLint p_instanced_kelp_uni_cameraPosition = -1;
    GLint p_instanced_kelp_uni_lightPosition = -1;
    GLint p_instanced_kelp_uni_lightTransformation = -1;
    GLint p_instanced_kelp_uni_textureSize = -1;
    GLint p_instanced_kelp_uni_time = -1;
    GLint p_instanced_kelp_uni_transformation = -1;
    GLint p_instanced_kelp_uni_waterHeight = -1;
    GLint p_instanced_kelp_uni_aoMap = -1;
    GLint p_instanced_kelp_uni_caustics = -1;
    GLint p_instanced_kelp_uni_matrices = -1;
    GLint p_instanced_kelp_uni_normalSampler = -1;
    GLint p_instanced_kelp_uni_texAlbedo = -1;
    // instanced_kelp_shadow:
    GLuint p_instanced_kelp_shadow = 0;
    bool p_instanced_kelp_shadow_loaded = false;
    GLint p_instanced_kelp_shadow_attr_vertexPosition = -1;
    GLint p_instanced_kelp_shadow_attr_vertexTex = -1;
    GLint p_instanced_kelp_shadow_attr_gl_InstanceID = -1;
    GLint p_instanced_kelp_shadow_uni_matrices = -1;
    GLint p_instanced_kelp_shadow_uni_texAlbedo = -1;
    GLint p_instanced_kelp_shadow_uni_textureSize = -1;
    GLint p_instanced_kelp_shadow_uni_time = -1;
    GLint p_instanced_kelp_shadow_uni_transformation = -1;
    

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
    void loadModels();
    void loadSounds();
    void loadAttributesAndUniforms();
    void loadTexture(const char *name, GLuint *out);
    void loadModel(const char *name, Model **out);
    void loadSound(const char *name, ALenum format, ALsizei freq, ALuint *out);
    void loadTextureCubeMap(GLuint *out);
    /**
     * true if and only if program was loaded right now
     */
    bool loadProgram(const char *name, GLuint *out_program, bool *out_loaded, int shadersCount, ...);
    GLuint compileShader(GLenum shaderType, const char *name);
    GLuint createProgram(const char *name, int shadersCount, GLuint *shaders);
};

extern ResourceLoader* resourceLoaderExternal;