#include <Logger.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <errno.h>
#include <png/png.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <ResourceLoader.hpp>
#include <Resources/Model.hpp>
#include <assimp/Importer.hpp>
#include <filesystem>
#include <iostream>
#include <unordered_set>

ResourceLoader::ResourceLoader() {
    resourceLoaderExternal = this;
}

ResourceLoader::~ResourceLoader() {
}

void ResourceLoader::loadTextures() {
    loadTextureCubeMap(&this->tex_skybox);

    // generated by: ./storage/textures.sh
    loadTexture("assets/textures/asteroid.png", &this->tex_asteroid);
    loadTexture("assets/textures/dummy.png", &this->tex_dummy);
    loadTexture("assets/textures/earth.png", &this->tex_earth);
    loadTexture("assets/textures/earth2.png", &this->tex_earth2);
    loadTexture("assets/textures/earth2_normals.png", &this->tex_earth2_normals);
    loadTexture("assets/textures/entities/pilotfish/pilotfish_albedo.png", &this->tex_entities_pilotfish_pilotfish_albedo);
    loadTexture("assets/textures/foliage/seagrass/dried_grass_albedo.png", &this->tex_foliage_seagrass_dried_grass_albedo);
    loadTexture("assets/textures/foliage/seagrass/dried_grass_height.png", &this->tex_foliage_seagrass_dried_grass_height);
    loadTexture("assets/textures/foliage/seagrass/dried_grass_normal.png", &this->tex_foliage_seagrass_dried_grass_normal);
    loadTexture("assets/textures/foliage/seagrass/grass_blades_albedo.png", &this->tex_foliage_seagrass_grass_blades_albedo);
    loadTexture("assets/textures/foliage/seagrass/grass_blades_ao.png", &this->tex_foliage_seagrass_grass_blades_ao);
    loadTexture("assets/textures/foliage/seagrass/grass_blades_normal.png", &this->tex_foliage_seagrass_grass_blades_normal);
    loadTexture("assets/textures/generic/Ground_Sand1_2x2_1K/Ground_Sand1_2x2_1K_albedo.png", &this->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_albedo);
    loadTexture("assets/textures/generic/Ground_Sand1_2x2_1K/Ground_Sand1_2x2_1K_ao.png", &this->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_ao);
    loadTexture("assets/textures/generic/Ground_Sand1_2x2_1K/Ground_Sand1_2x2_1K_height.png", &this->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_height);
    loadTexture("assets/textures/generic/Ground_Sand1_2x2_1K/Ground_Sand1_2x2_1K_normal.png", &this->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_normal);
    loadTexture("assets/textures/generic/Ground_Sand1_2x2_1K/Ground_Sand1_2x2_1K_roughness.png", &this->tex_generic_Ground_Sand1_2x2_1K_Ground_Sand1_2x2_1K_roughness);
    loadTexture("assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_albedo.png", &this->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_albedo);
    loadTexture("assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_ao.png", &this->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_ao);
    loadTexture("assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_height.png", &this->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_height);
    loadTexture("assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_normal.png", &this->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_normal);
    loadTexture("assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_roughness.png", &this->tex_generic_Nature_Grass1_2x2_1K_Nature_Grass_1K_roughness);
    loadTexture("assets/textures/generic/Nature_SandBeach1_2x2_1K/Nature_SandBeach_1K_albedo.png", &this->tex_generic_Nature_SandBeach1_2x2_1K_Nature_SandBeach_1K_albedo);       
    loadTexture("assets/textures/generic/Nature_SandBeach1_2x2_1K/Nature_SandBeach_1K_height.png", &this->tex_generic_Nature_SandBeach1_2x2_1K_Nature_SandBeach_1K_height);       
    loadTexture("assets/textures/generic/Nature_SandBeach1_2x2_1K/Nature_SandBeach_1K_normal.png", &this->tex_generic_Nature_SandBeach1_2x2_1K_Nature_SandBeach_1K_normal);       
    loadTexture("assets/textures/generic/Nature_SandBeach1_2x2_1K/Nature_SandBeach_1K_roughness.png", &this->tex_generic_Nature_SandBeach1_2x2_1K_Nature_SandBeach_1K_roughness); 
    loadTexture("assets/textures/generic/Sand_Muddy2_2x2_1K/Sand_Muddy2_2x2_1K_albedo.png", &this->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_albedo);
    loadTexture("assets/textures/generic/Sand_Muddy2_2x2_1K/Sand_Muddy2_2x2_1K_ao.png", &this->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_ao);
    loadTexture("assets/textures/generic/Sand_Muddy2_2x2_1K/Sand_Muddy2_2x2_1K_height.png", &this->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_height);
    loadTexture("assets/textures/generic/Sand_Muddy2_2x2_1K/Sand_Muddy2_2x2_1K_normal.png", &this->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_normal);
    loadTexture("assets/textures/generic/Sand_Muddy2_2x2_1K/Sand_Muddy2_2x2_1K_roughness.png", &this->tex_generic_Sand_Muddy2_2x2_1K_Sand_Muddy2_2x2_1K_roughness);
    loadTexture("assets/textures/generic/Sand_Muddy7_3x3_1K/Sand_Muddy7_3x3_1K_albedo.png", &this->tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_albedo);
    loadTexture("assets/textures/generic/Sand_Muddy7_3x3_1K/Sand_Muddy7_3x3_1K_ao.png", &this->tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_ao);
    loadTexture("assets/textures/generic/Sand_Muddy7_3x3_1K/Sand_Muddy7_3x3_1K_height.png", &this->tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_height);
    loadTexture("assets/textures/generic/Sand_Muddy7_3x3_1K/Sand_Muddy7_3x3_1K_normal.png", &this->tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_normal);
    loadTexture("assets/textures/generic/Sand_Muddy7_3x3_1K/Sand_Muddy7_3x3_1K_roughness.png", &this->tex_generic_Sand_Muddy7_3x3_1K_Sand_Muddy7_3x3_1K_roughness);
    loadTexture("assets/textures/grid.png", &this->tex_grid);
    loadTexture("assets/textures/grid_color.png", &this->tex_grid_color);
    loadTexture("assets/textures/moon.png", &this->tex_moon);
    loadTexture("assets/textures/moon_normals.png", &this->tex_moon_normals);
    loadTexture("assets/textures/rocks/rock_big1/rock_big1_albedo.png", &this->tex_rocks_rock_big1_rock_big1_albedo);
    loadTexture("assets/textures/rocks/rock_big1/rock_big1_ao.png", &this->tex_rocks_rock_big1_rock_big1_ao);
    loadTexture("assets/textures/rocks/rock_big1/rock_big1_height.png", &this->tex_rocks_rock_big1_rock_big1_height);
    loadTexture("assets/textures/rocks/rock_big1/rock_big1_normal.png", &this->tex_rocks_rock_big1_rock_big1_normal);
    loadTexture("assets/textures/spaceship.png", &this->tex_spaceship);
    loadTexture("assets/textures/spaceship_normals.png", &this->tex_spaceship_normals);
    loadTexture("assets/textures/test.png", &this->tex_test);
    loadTexture("assets/textures/test_normals.png", &this->tex_test_normals);
    loadTexture("assets/textures/uv_grid.png", &this->tex_uv_grid);
    loadTexture("assets/textures/wall.png", &this->tex_wall);
    loadTexture("assets/textures/wall_ao.png", &this->tex_wall_ao);
    loadTexture("assets/textures/wall_height.png", &this->tex_wall_height);
    loadTexture("assets/textures/wall_normal.png", &this->tex_wall_normal);
    loadTexture("assets/textures/wall_roughness.png", &this->tex_wall_roughness);
}

void ResourceLoader::loadPrograms() {
#define LOAD_PROGRAM(name, count, ...) if (loadProgram(#name, &this->p_##name, &this->p_##name##_loaded, count, __VA_ARGS__))
#define ATTRIBUTE(shaderName, attrName) this->p_##shaderName##_attr_##attrName = glGetAttribLocation(this->p_##shaderName, #attrName);
#define UNIFORM(shaderName, attrName) this->p_##shaderName##_uni_##attrName = glGetUniformLocation(this->p_##shaderName, #attrName);
#define DUMP(shaderName) this->dumpProgram(#shaderName, this->p_##shaderName);

    LOAD_PROGRAM(color_armature, 2, "armature/shader_color_armature.frag", "armature/shader_color_armature.vert") {
        // DUMP(color_armature);
        ATTRIBUTE(color_armature, vertexJoints);
        ATTRIBUTE(color_armature, vertexPosition);
        ATTRIBUTE(color_armature, vertexWeights);
        UNIFORM(color_armature, jointTransforms);
        UNIFORM(color_armature, modelViewProjectionMatrix);
    }

    LOAD_PROGRAM(simple_color_shader, 2, "debug/simple_color_shader.frag", "debug/simple_color_shader.vert") {
        // DUMP(simple_color_shader);
        ATTRIBUTE(simple_color_shader, vertexColor);
        ATTRIBUTE(simple_color_shader, vertexPosition);
        UNIFORM(simple_color_shader, transformation);
    }

    LOAD_PROGRAM(simple_uni_color_shader, 2, "debug/simple_uni_color_shader.frag", "debug/simple_uni_color_shader.vert") {
        // DUMP(simple_uni_color_shader);
        ATTRIBUTE(simple_uni_color_shader, vertexPosition);
        UNIFORM(simple_uni_color_shader, color);
        UNIFORM(simple_uni_color_shader, transformation);
    }

    LOAD_PROGRAM(simple_tex_shader, 2, "debug/simple_tex_shader.frag", "debug/simple_tex_shader.vert") {
        // DUMP(simple_tex_shader);
        ATTRIBUTE(simple_tex_shader, vertexPosition);
        ATTRIBUTE(simple_tex_shader, vertexTex);
        UNIFORM(simple_tex_shader, textureSampler);
        UNIFORM(simple_tex_shader, transformation);
    }

    LOAD_PROGRAM(skybox_shader, 2, "cubemap/cubemap.frag", "cubemap/cubemap.vert") {
        // DUMP(skybox_shader);
        ATTRIBUTE(skybox_shader, aPos);
        UNIFORM(skybox_shader, projection);
        UNIFORM(skybox_shader, skybox);
    }

    LOAD_PROGRAM(underwater_fog_shader, 2, "fog/underwaterFog.frag", "fog/underwaterFog.vert") {
        // DUMP(underwater_fog_shader);
        ATTRIBUTE(underwater_fog_shader, aPos);
        ATTRIBUTE(underwater_fog_shader, vertexTexCoord);
        UNIFORM(underwater_fog_shader, distanceToSurface);
        UNIFORM(underwater_fog_shader, depthTexture);
        UNIFORM(underwater_fog_shader, screenTexture);
    }

    LOAD_PROGRAM(shader_4_1, 2, "shader_4_1.frag", "shader_4_1.vert") {
        // DUMP(shader_4_1);
        ATTRIBUTE(shader_4_1, vertexNormal);
        ATTRIBUTE(shader_4_1, vertexPosition);
        UNIFORM(shader_4_1, cameraPos);
        UNIFORM(shader_4_1, lightPos);
        UNIFORM(shader_4_1, modelMatrix);
        UNIFORM(shader_4_1, objectColor);
        UNIFORM(shader_4_1, transformation);
    }
    LOAD_PROGRAM(shader_4_sun, 2, "shader_4_sun.frag", "shader_4_sun.vert") {
        // DUMP(shader_4_sun);
        ATTRIBUTE(shader_4_sun, vertexNormal);
        ATTRIBUTE(shader_4_sun, vertexPosition);
        UNIFORM(shader_4_sun, cameraPos);
        UNIFORM(shader_4_sun, modelMatrix);
        UNIFORM(shader_4_sun, modelViewProjectionMatrix);
        UNIFORM(shader_4_sun, objectColor);
    }
    LOAD_PROGRAM(shader_4_tex, 2, "shader_4_tex.frag", "shader_4_tex.vert") {
        // DUMP(shader_4_tex);
        ATTRIBUTE(shader_4_tex, vertexBitangent);
        ATTRIBUTE(shader_4_tex, vertexNormal);
        ATTRIBUTE(shader_4_tex, vertexPosition);
        ATTRIBUTE(shader_4_tex, vertexTangent);
        ATTRIBUTE(shader_4_tex, vertexTexCoord);
        UNIFORM(shader_4_tex, cameraPos);
        UNIFORM(shader_4_tex, colorTexture);
        UNIFORM(shader_4_tex, lightPos);
        UNIFORM(shader_4_tex, modelMatrix);
        UNIFORM(shader_4_tex, normalSampler);
        UNIFORM(shader_4_tex, transformation);
    }
    LOAD_PROGRAM(shader_4_tex_with_parallax, 2, "shader_4_tex_with_parallax.frag", "shader_4_tex.vert") {
        // DUMP(shader_4_tex_with_parallax);
        ATTRIBUTE(shader_4_tex_with_parallax, vertexBitangent);
        ATTRIBUTE(shader_4_tex_with_parallax, vertexNormal);
        ATTRIBUTE(shader_4_tex_with_parallax, vertexPosition);
        ATTRIBUTE(shader_4_tex_with_parallax, vertexTangent);
        ATTRIBUTE(shader_4_tex_with_parallax, vertexTexCoord);
        UNIFORM(shader_4_tex_with_parallax, cameraPos);
        UNIFORM(shader_4_tex_with_parallax, colorTexture);
        UNIFORM(shader_4_tex_with_parallax, depthSampler);
        UNIFORM(shader_4_tex_with_parallax, heightScale);
        UNIFORM(shader_4_tex_with_parallax, lightPos);
        UNIFORM(shader_4_tex_with_parallax, modelMatrix);
        UNIFORM(shader_4_tex_with_parallax, normalSampler);
        UNIFORM(shader_4_tex_with_parallax, transformation);
    }

    LOAD_PROGRAM(shader_color, 2, "shader_color.frag", "shader_color.vert") {
        // DUMP(shader_color);
        ATTRIBUTE(shader_color, vertexNormal);
        ATTRIBUTE(shader_color, vertexPosition);
        UNIFORM(shader_color, lightDir);
        UNIFORM(shader_color, modelMatrix);
        UNIFORM(shader_color, modelViewProjectionMatrix);
        UNIFORM(shader_color, objectColor);
    }

    LOAD_PROGRAM(shader_tex, 2, "shader_tex.frag", "shader_tex.vert") {
        // DUMP(shader_tex);
        ATTRIBUTE(shader_tex, vertexNormal);
        ATTRIBUTE(shader_tex, vertexPosition);
        ATTRIBUTE(shader_tex, vertexTexCoord);
        UNIFORM(shader_tex, lightDir);
        UNIFORM(shader_tex, modelMatrix);
        UNIFORM(shader_tex, modelViewProjectionMatrix);
        UNIFORM(shader_tex, textureSampler);
    }

    // WATER SURFACE
    LOAD_PROGRAM(water_simulation, 2, "water/simulation.frag", "water/simulation.vert") {
        // DUMP(water_simulation);
        ATTRIBUTE(water_simulation, vertexPosition);
        UNIFORM(water_simulation, scale);
        UNIFORM(water_simulation, time);
        UNIFORM(water_simulation, transition);
        UNIFORM(water_simulation, waveCount);
        UNIFORM(water_simulation, waves);
    }
    LOAD_PROGRAM(water_surface, 2, "water/surface.frag", "water/surface.vert") {
        // DUMP(water_surface);
        ATTRIBUTE(water_surface, position);
        ATTRIBUTE(water_surface, texturePosition);
        UNIFORM(water_surface, cameraPosition);
        UNIFORM(water_surface, heightMap);
        UNIFORM(water_surface, model);
        UNIFORM(water_surface, normalMap);
        UNIFORM(water_surface, skybox);
        UNIFORM(water_surface, transformation);
    }

    LOAD_PROGRAM(simplex, 2, "simplex.frag", "simplex.vert") {
        // DUMP(simplex);
        ATTRIBUTE(simplex, pos);
        ATTRIBUTE(simplex, tex);
        UNIFORM(simplex, p);
        UNIFORM(simplex, scale);
        UNIFORM(simplex, translation);
        UNIFORM(simplex, layer);
    }
    LOAD_PROGRAM(environment_map, 2, "water/environment.frag", "water/environment.vert") {
        // DUMP(environment_map);
        ATTRIBUTE(environment_map, vertexPosition);
        UNIFORM(environment_map, modelMatrix);
        UNIFORM(environment_map, transformation);
    }
    LOAD_PROGRAM(caustics, 2, "water/caustics.frag", "water/caustics.vert") {
        // DUMP(caustics);
        ATTRIBUTE(caustics, texturePosition);
        ATTRIBUTE(caustics, vertexPosition);
        UNIFORM(caustics, environmentMap);
        UNIFORM(caustics, height);
        UNIFORM(caustics, heightMap);
        UNIFORM(caustics, lightDirection);
        UNIFORM(caustics, modelMatrix);
        UNIFORM(caustics, normalMap);
        UNIFORM(caustics, transformation);
    }
    LOAD_PROGRAM(chunk, 2, "chunk.frag", "chunk.vert") {
        // DUMP(chunk);
        ATTRIBUTE(chunk, vertexBitangent);
        ATTRIBUTE(chunk, vertexNormal);
        ATTRIBUTE(chunk, vertexPosition);
        ATTRIBUTE(chunk, vertexTangent);
        ATTRIBUTE(chunk, vertexTexCoord);
        UNIFORM(chunk, aoMap);
        UNIFORM(chunk, cameraPosition);
        UNIFORM(chunk, caustics);
        UNIFORM(chunk, envMap);
        UNIFORM(chunk, colorTexture);
        UNIFORM(chunk, depthMap);
        UNIFORM(chunk, lightPosition);
        UNIFORM(chunk, lightTransformation);
        UNIFORM(chunk, modeSwitch);
        UNIFORM(chunk, modelMatrix);
        UNIFORM(chunk, normalSampler);
        UNIFORM(chunk, roughnessMap);
        UNIFORM(chunk, transformation);
        UNIFORM(chunk, waterHeight);
    }
    LOAD_PROGRAM(pilotfish, 2, "fish/pilotfish.frag", "fish/pilotfish.vert") {
        // DUMP(pilotfish);
        ATTRIBUTE(pilotfish, vertexBitangent);
        ATTRIBUTE(pilotfish, vertexNormal);
        ATTRIBUTE(pilotfish, vertexPosition);
        ATTRIBUTE(pilotfish, vertexTangent);
        ATTRIBUTE(pilotfish, vertexTexCoord);
        UNIFORM(pilotfish, cameraPosition);
        UNIFORM(pilotfish, lightPosition);
        UNIFORM(pilotfish, lightTransformation);
        UNIFORM(pilotfish, modelMatrix);
        UNIFORM(pilotfish, transformation);
        UNIFORM(pilotfish, waterHeight);
        UNIFORM(pilotfish, caustics);
        UNIFORM(pilotfish, envMap);
        UNIFORM(pilotfish, colorTexture);
    }
    LOAD_PROGRAM(cubefish, 2, "fish/cubefish.frag", "fish/cubefish.vert") {
        // DUMP(cubefish);
        ATTRIBUTE(cubefish, vertexBitangent);
        ATTRIBUTE(cubefish, vertexNormal);
        ATTRIBUTE(cubefish, vertexPosition);
        ATTRIBUTE(cubefish, vertexTangent);
        ATTRIBUTE(cubefish, vertexTexCoord);
        UNIFORM(cubefish, cameraPosition);
        UNIFORM(cubefish, lightPosition);
        UNIFORM(cubefish, lightTransformation);
        UNIFORM(cubefish, modelMatrix);
        UNIFORM(cubefish, transformation);
        UNIFORM(cubefish, waterHeight);
        UNIFORM(cubefish, aoMap);
        UNIFORM(cubefish, caustics);
        UNIFORM(cubefish, envMap);
        UNIFORM(cubefish, colorTexture);
        UNIFORM(cubefish, depthMap);
        UNIFORM(cubefish, normalSampler);
        UNIFORM(cubefish, roughnessMap);
    }
    LOAD_PROGRAM(glow, 2, "glow.frag", "glow.vert") {
        // DUMP(glow);
        ATTRIBUTE(glow, pos);
        ATTRIBUTE(glow, tex);
        UNIFORM(glow, color);
        UNIFORM(glow, divider);
        UNIFORM(glow, maxSample);
        UNIFORM(glow, radius);
        UNIFORM(glow, glowTexture);
        UNIFORM(glow, texelSize);
    }

#undef LOAD_PROGRAM
#undef ATTRIBUTE
#undef UNIFORM
}

void ResourceLoader::loadModels() {
    loadModel("assets/models/primitives/AnimatedStick.dae", &this->m_primitives_AnimatedStick);
    loadModel("assets/models/primitives/complex_cube.dae", &this->m_primitives_complex_cube);
    loadModel("assets/models/primitives/cube.obj", &this->m_primitives_cube);
    loadModel("assets/models/primitives/cylinder.obj", &this->m_primitives_cylinder);
    loadModel("assets/models/primitives/plane.dae", &this->m_primitives_plane);
    loadModel("assets/models/primitives/sphere.obj", &this->m_primitives_sphere);
    loadModel("assets/models/robot.dae", &this->m_robot);
    loadModel("assets/models/robot_with_armature.dae", &this->m_robot_with_armature);
    loadModel("assets/models/spaceship.obj", &this->m_spaceship);
    loadModel("assets/models/sphere.obj", &this->m_sphere);
    loadModel("assets/models/sphere2.obj", &this->m_sphere2);
    loadModel("assets/models/sphere_different_texcoords.obj", &this->m_sphere_different_texcoords);
    loadModel("assets/models/entities/pilotfish.dae", &this->m_pilotfish);
}

bool ResourceLoader::loadResources() {
    if (this->all_loaded) {
        return true;
    }
    this->loadedResources = 0;
    this->totalResourcesCounter = 0;

    if (this->totalResources == 0) {
        // first frame - count all resources
        this->startFrame = 0;
    } else {
        this->startFrame = glfwGetTime();
    }

    this->loadTextures();
    this->loadPrograms();
    this->loadModels();

    this->totalResources = this->totalResourcesCounter;

    if (this->totalResources == this->loadedResources) {
        if (this->shadersCap > 0) {
            this->shadersCap = 0;
            free(this->shaders);
        }
        LOGI("[ %3.0f%% ] Done", this->loadedResources * 100.0 / this->totalResources);
        this->all_loaded = true;
        return true;
    }
    return false;
}

bool ResourceLoader::canLoadNextResource() {
    if (this->startFrame == 0) {
        // optimize - don't check time & don't load more resources
        return false;
    }
    if (this->totalResourcesCounter == this->loadedResources) {
        // load minimum one resource
        return true;
    }
    // max 100 ms per frame
    if (glfwGetTime() - this->startFrame < 0.00001) {
        return true;
    } else {
        this->startFrame = 0;
        return false;
    }
}

void ResourceLoader::loadTextureCubeMap(GLuint *out) {
    this->totalResourcesCounter++;
    if (*out) {
        this->loadedResources++;
        return;
    }
    if (!canLoadNextResource()) {
        return;
    }
    LOGI("[ %3.0f%% ] Loading tex: %s", this->loadedResources * 100.0 / this->totalResources, "CubeMap");
    const char *names[] = {"assets/textures/skybox/left.png", "assets/textures/skybox/right.png", "assets/textures/skybox/top.png", "assets/textures/skybox/bottom.png", "assets/textures/skybox/front.png", "assets/textures/skybox/back.png"};
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        png_uint_32 w, h;
        png_byte color_type, bit_depth;
        png_structp png;
        png_infop info;
        size_t row;
        png_byte *image;
        png_bytep *row_pointers;

        FILE *fp = fopen(names[i], "rb");
        if (!fp) {
            LOGE("fopen failed");
            goto done;
        }
        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png) {
            LOGE("png_create_read_struct failed");
            goto done;
        }
        info = png_create_info_struct(png);
        if (!info) {
            LOGE("png_create_info_struct failed");
            goto done;
        }
        if (setjmp(png_jmpbuf(png))) {
            LOGE("setjmp(png_jmpbuf(png)) failed");
            goto done;
        }
        png_init_io(png, fp);
        png_read_info(png, info);

        w = png_get_image_width(png, info);
        h = png_get_image_height(png, info);
        color_type = png_get_color_type(png, info);
        bit_depth = png_get_bit_depth(png, info);

        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png);
        }
        if (png_get_valid(png, info, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(png);
        }
        png_read_update_info(png, info);
        color_type = png_get_color_type(png, info);
        bit_depth = png_get_bit_depth(png, info);

        row = png_get_rowbytes(png, info);
        image = new png_byte[row * h];
        row_pointers = new png_bytep[h];
        for (png_uint_32 y = 0; y < h; y++) {
            row_pointers[y] = image + row * y;
        }
        png_read_image(png, row_pointers);

        if (color_type == PNG_COLOR_TYPE_RGB_ALPHA && bit_depth == 8) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        } else if (color_type == PNG_COLOR_TYPE_RGB && bit_depth == 8) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        } else {
            LOGE("Unsupported color_type: %d / bit_depth: %d", color_type, bit_depth);
        }

        delete[] image;
        delete[] row_pointers;

    done:
        if (png && info) {
            png_destroy_read_struct(&png, &info, NULL);
        }
        if (fp) {
            fclose(fp);
        }
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    *out = id;
    this->loadedResources++;
}

void ResourceLoader::loadTexture(const char *name, GLuint *out) {
    this->totalResourcesCounter++;
    if (*out) {
        this->loadedResources++;
        return;
    }
    if (!canLoadNextResource()) {
        return;
    }
    LOGI("[ %3.0f%% ] Loading tex: %s", this->loadedResources * 100.0 / this->totalResources, name);
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    png_uint_32 w, h;
    png_byte color_type, bit_depth;
    png_structp png;
    png_infop info;
    size_t row;
    png_byte *image;
    png_bytep *row_pointers;

    FILE *fp = fopen(name, "rb");
    if (!fp) {
        LOGE("fopen failed");
        goto done;
    }
    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        LOGE("png_create_read_struct failed");
        goto done;
    }
    info = png_create_info_struct(png);
    if (!info) {
        LOGE("png_create_info_struct failed");
        goto done;
    }
    if (setjmp(png_jmpbuf(png))) {
        LOGE("setjmp(png_jmpbuf(png)) failed");
        goto done;
    }
    png_init_io(png, fp);
    png_read_info(png, info);

    w = png_get_image_width(png, info);
    h = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }
    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_add_alpha(png, (1 << bit_depth) - 1, PNG_FILLER_AFTER);
    }
    png_set_swap(png);
    png_read_update_info(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);

    row = png_get_rowbytes(png, info);
    image = new png_byte[row * h];
    row_pointers = new png_bytep[h];
    for (png_uint_32 y = 0; y < h; y++) {
        row_pointers[y] = image + row * y;
    }
    png_read_image(png, row_pointers);

    if (bit_depth == 16) {
        png_uint_32 colors = (png_uint_32)(row * h * 8 / (png_uint_32)bit_depth);
        png_uint_16 *old_image = (png_uint_16 *)image;
        float *new_image = new float[colors];
        for (png_uint_32 i = 0; i < colors; i++) {
            new_image[i] = old_image[i] / 65535.0f;
        }
        if (color_type == PNG_COLOR_TYPE_GRAY) {  // 0
            LOGD("16-bit RED");
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, w, h, 0, GL_RED, GL_FLOAT, new_image);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {  // 4
            LOGD("16-bit RED GREEN");
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, w, h, 0, GL_RG, GL_FLOAT, new_image);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {  // 6
            LOGD("16-bit RGBA");
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, new_image);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            LOGE("Unsupported color_type: %d", color_type);
        }

        delete[] new_image;
    } else if (bit_depth == 8) {
        if (color_type == PNG_COLOR_TYPE_GRAY) {  // 0
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {  // 4
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, w, h, 0, GL_RG, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {  // 6
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            LOGE("Unsupported color_type: %d", color_type);
        }
    } else {
        LOGE("Unsupported bit_depth: %d", bit_depth);
    }

    delete[] image;
    delete[] row_pointers;

done:
    if (png && info) {
        png_destroy_read_struct(&png, &info, NULL);
    }
    if (fp) {
        fclose(fp);
    }
    *out = id;
    this->loadedResources++;
}

bool ResourceLoader::loadProgram(const char *name, GLuint *out_program, bool *out_loaded, int shadersCount, ...) {
    this->totalResourcesCounter += shadersCount + 1;
    if (*out_loaded) {
        this->loadedResources += shadersCount + 1;
        return false;
    }
    if (!canLoadNextResource()) {
        return false;
    }
    if (this->shadersCap < shadersCount) {
        GLuint *newShaders = (GLuint *)realloc(this->shaders, shadersCount * sizeof(GLuint));
        if (newShaders == 0) {
            LOGE("realloc(%d) failed: %d (%s)", shadersCount * sizeof(GLuint), errno, strerror(errno));
            return false;
        }
        this->shaders = newShaders;
        this->shadersCap = shadersCount;
    }

    va_list ap;
    va_start(ap, shadersCount);
    for (int i = 0; i < shadersCount; i++) {
        if (i < this->shadersCompiled) {
            va_arg(ap, const char *);
            this->loadedResources++;
            continue;
        }
        if (!canLoadNextResource()) {
            va_end(ap);
            return false;
        }
        const char *shaderName = va_arg(ap, const char *);
        size_t shaderNameLen = strlen(shaderName);
        char *fullPath = (char *)malloc(shaderNameLen + sizeof("assets/shaders/"));
        memcpy(fullPath, "assets/shaders/", sizeof("assets/shaders/"));
        memcpy(fullPath + sizeof("assets/shaders/") - 1, shaderName, shaderNameLen + 1);
        LOGI("[ %3.0f%% ] Loading shader: %s", this->loadedResources * 100.0 / this->totalResources, shaderName);
        this->shaders[i] = compileShader(i % 2 != 0 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER, fullPath);
        this->shadersCompiled++;
        this->loadedResources++;
    }
    va_end(ap);

    if (!canLoadNextResource()) {
        return false;
    }

    if (this->shadersCompiled == shadersCount)  // always true?
    {
        LOGI("[ %3.0f%% ] Loading program: %s", this->loadedResources * 100.0 / this->totalResources, name);
        *out_loaded = true;
        *out_program = this->createProgram(name, shadersCount, this->shaders);
        this->shadersCompiled = 0;
        this->loadedResources++;
        return true;
    }
    return false;
}

void ResourceLoader::loadModel(const char *name, Model **out) {
    this->totalResourcesCounter++;
    if (*out) {
        this->loadedResources++;
        return;
    }
    if (!canLoadNextResource()) {
        return;
    }
    LOGI("[ %3.0f%% ] Loading model: %s", this->loadedResources * 100.0 / this->totalResources, name);
    *out = new Model();
    (*out)->loadModel(name);
}

char *ResourceLoader::readFile(const char *file, size_t *size) {
    FILE *f = fopen(file, "rb");
    if (!f) {
        LOGE("fopen(%s): %d (%s)", file, errno, strerror(errno));
        return 0;
    }
    goto readFile;
closeFile:
    if (fclose(f)) {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    *size = 0;
    return 0;
readFile:
    if (fseek(f, 0, SEEK_END)) {
        LOGE("fseek(f, 0, SEEK_END): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    long int s = ftell(f);
    if (s <= (long int)0) {
        LOGE("ftell(f): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    if (fseek(f, 0, SEEK_SET)) {
        LOGE("fseek(f, 0, SEEK_SET): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    char *mem = (char *)malloc(s);
    *size = s;
    size_t r = fread(mem, 1, s, f);
    if (r != s) {
        LOGE("fread %d != %d (ret, size): %d (%s)", r, s, errno, strerror(errno));
        free(mem);
        goto closeFile;
    }
    if (fclose(f)) {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    return mem;
}

void ResourceLoader::saveFile(const char *file, const char *data, size_t size) {
    FILE *f = fopen(file, "wb");
    if (!f) {
        LOGE("fopen(%s): %d (%s)", file, errno, strerror(errno));
        return;
    }
    goto writeFile;
closeFile:
    if (fclose(f)) {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    return;
writeFile:
    size_t r = fwrite(data, 1, size, f);
    if (r != size) {
        LOGE("fwrite %d != %d (ret, size): %d (%s)", r, size, errno, strerror(errno));
        goto closeFile;
    }
    if (fclose(f)) {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    return;
}

GLuint ResourceLoader::compileShader(GLenum shaderType, const char *name) {
    size_t size;
    char *file = this->readFile(name, &size);
    if (!file) {
        return 0;
    }
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        LOGE("glCreateShader failed");
        free(file);
        return 0;
    }
    const GLint length = (const GLint)size;
    glShaderSource(shader, 1, &file, &length);
    glCompileShader(shader);
    free(file);
    GLint compile_result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);
    if (compile_result == GL_FALSE) {
        LOGE("Error compiling shader %s", name);
        GLint info_log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        char *mem = (char *)malloc(info_log_length);
        if (mem) {
            glGetShaderInfoLog(shader, info_log_length, NULL, mem);
            LOGE("Logs: %s", mem);
            free(mem);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint ResourceLoader::createProgram(const char *name, int shadersCount, GLuint *shaders) {
    GLuint program = glCreateProgram();
    if (program == 0) {
        LOGE("glCreateProgram failed, unloading shaders");
        for (int i = 0; i < shadersCount; i++) {
            if (shaders[i]) {
                glDeleteShader(shaders[i]);
            }
        }
        return 0;
    }
    for (int i = 0; i < shadersCount; i++) {
        if (shaders[i]) {
            glAttachShader(program, shaders[i]);
        }
    }
    glLinkProgram(program);

    for (int i = 0; i < shadersCount; i++) {
        if (shaders[i]) {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }
    }

    GLint link_result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &link_result);
    if (link_result == GL_FALSE) {
        LOGE("Error linking program %s", name);
        GLint info_log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        char *mem = (char *)malloc(info_log_length);
        if (mem) {
            glGetProgramInfoLog(program, info_log_length, NULL, mem);
            LOGE("Logs: %s", mem);
            free(mem);
        }
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void ResourceLoader::dumpProgram(const char *name, GLuint program) {
    if (program == 0) {
        LOGW("Invalid program: %s", name);
        return;
    }
    printf("Header code:\n");
    printf("    // %s:\n", name);
    glUseProgram(program);
    GLint i;
    GLint count;
    GLint size;
    GLenum type;
    printf("    GLuint p_%s = 0;\n", name);
    printf("    bool p_%s_loaded = false;\n", name);
    GLint bufSize = 16;
    GLsizei length;
    GLchar *glname;

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++) {
        glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("    GLint p_%s_attr_%s = -1;\n",
               name, glname);
    }
    free(glname);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++) {
        glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("    GLint p_%s_uni_%s = -1;\n",
               name, glname);
    }
    printf("\n");
    free(glname);

    printf("Source code:\n");
    printf("    {\n");

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++) {
        glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("        ATTRIBUTE(%s, %s);\n", name, glname);
    }
    free(glname);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++) {
        glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("        UNIFORM(%s, %s);\n", name, glname);
    }
    free(glname);

    printf("    }\n");
}

ResourceLoader *resourceLoaderExternal;