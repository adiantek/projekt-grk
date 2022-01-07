#include <iostream>
#include <string>

#include <ResourceLoader.hpp>
#include <Resources/Resources.hpp>
#include <Resources/Material.hpp>

void Resources::init() {
    Resources::initTextures();
    Resources::initMaterials();
    Resources::initShaders();
    Resources::initModels();
}

//////////////////////////////////////////////////////////////////////////
//---------------------------- TEXTURES --------------------------------//
//////////////////////////////////////////////////////////////////////////
void Resources::initTextures() {
    // Robot textures
    ResourceLoader::loadTextureExternal("assets/textures/robot/body/diffuse.png", &Resources::TEXTURES.ROBOT_BODY_DIFFUSE);
    ResourceLoader::loadTextureExternal("assets/textures/robot/body/ao.png", &Resources::TEXTURES.ROBOT_BODY_AO);
    ResourceLoader::loadTextureExternal("assets/textures/robot/body/normal.png", &Resources::TEXTURES.ROBOT_BODY_NORMAL);
    ResourceLoader::loadTextureExternal("assets/textures/robot/body/roughness.png", &Resources::TEXTURES.ROBOT_BODY_ROUGHNESS);
}

///////////////////////////////////////////////////////////////////////////
//---------------------------- MATERIALS --------------------------------//
///////////////////////////////////////////////////////////////////////////
void Resources::initMaterials() {
    printf("Initializing materials...\n");

    Resources::MATERIALS.DEFAULT
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(0.827f, 0.827f, 0.827f));

    Resources::MATERIALS.DEFAULT_WHITE
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

    Resources::MATERIALS.DEFAULT_RED
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));

    Resources::MATERIALS.DEFAULT_GREEN
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));
    
    Resources::MATERIALS.DEFAULT_BLUE
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));

    Resources::MATERIALS.DEFAULT_YELLOW
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(1.0f, 1.0f, 0.0f));
    
    Resources::MATERIALS.DEFAULT_CYAN
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(0.0f, 1.0f, 1.0f));
    
    Resources::MATERIALS.DEFAULT_MAGENTA
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(1.0f, 0.0f, 1.0f));

    Resources::MATERIALS.DEFAULT_BLACK
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(0.0f, 0.0f, 0.0f));
    
    Resources::MATERIALS.DEFAULT_GREY
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(0.5f, 0.5f, 0.5f));

    Resources::MATERIALS.DEFAULT_ORANGE
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));

    Resources::MATERIALS.DEFAULT_PURPLE
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->setParam("objectColor", glm::vec3(0.5f, 0.0f, 1.0f));

    // Robot materials
    Resources::MATERIALS.ROBOT_BODY = new Material(&resourceLoaderExternal->p_shader_color, {
        {Material::DIFFUSE_TEXTURE, Resources::TEXTURES.ROBOT_BODY_DIFFUSE},
        {Material::AO_TEXTURE, Resources::TEXTURES.ROBOT_BODY_AO},
        {Material::NORMAL_TEXTURE, Resources::TEXTURES.ROBOT_BODY_NORMAL},
        {Material::ROUGHNESS_TEXTURE, Resources::TEXTURES.ROBOT_BODY_ROUGHNESS}
    });

    Resources::MATERIALS.ROBOT_BODY
        ->setParam("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));

    // Resources::MATERIALS.ROBOT_METAL = new Material(
    //     {{Material::DIFFUSE_TEXTURE, Resources::TEXTURES.ROBOT_METAL_DIFFUSE},
    //      {Material::AO_TEXTURE, Resources::TEXTURES.ROBOT_METAL_AO},
    //      {Material::NORMAL_TEXTURE, Resources::TEXTURES.ROBOT_METAL_NORMAL},
    //      {Material::ROUGHNESS_TEXTURE, Resources::TEXTURES.ROBOT_METAL_ROUGHNESS}});
}

///////////////////////////////////////////////////////////////////////////
//------------------------------ MODELS ---------------------------------//
///////////////////////////////////////////////////////////////////////////
void Resources::initModels() {

    std::cout << "Loading models..." << std::endl;

    // Primitives
    ResourceLoader::loadModelExternal("assets/models/primitives/plane.dae", Resources::MODELS.PLANE);
    ResourceLoader::loadModelExternal("assets/models/primitives/cube.dae", Resources::MODELS.CUBE);
    ResourceLoader::loadModelExternal("assets/models/primitives/complex_cube.dae", Resources::MODELS.COMPLEX_CUBE);


    // Robot
    ResourceLoader::loadModelExternal("assets/models/robot.dae", Resources::MODELS.ROBOT);
    std::cout << "Loaded: " << Resources::MODELS.ROBOT->file << std::endl;

}

///////////////////////////////////////////////////////////////////////////
//----------------------------- SHADERS ---------------------------------//
///////////////////////////////////////////////////////////////////////////
void Resources::initShaders() {
}
