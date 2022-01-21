#include <iostream>
#include <string>

#include <Logger.h>
#include <ResourceLoader.hpp>
#include <Resources/Resources.hpp>
#include <Resources/Material.hpp>

void Resources::init() {
    Resources::initMaterials();
    Resources::initShaders();
    Resources::initModels();
}

///////////////////////////////////////////////////////////////////////////
//---------------------------- MATERIALS --------------------------------//
///////////////////////////////////////////////////////////////////////////
void Resources::initMaterials() {
    LOGI("Initializing materials...");

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
    Resources::MATERIALS.ROBOT_BODY
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->setParam("objectColor", glm::vec3(0.107f, 0.084f, 0.064f));

    // TODO: Make a chrome shader for this
    Resources::MATERIALS.ROBOT_CHROME
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->setParam("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

    // TODO: Check for this material. I think robot has no usage for this
    Resources::MATERIALS.ROBOT_GLOW
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->setParam("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

    Resources::MATERIALS.ROBOT_LENS
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->setParam("objectColor", glm::vec3(0.207f, 0.184f, 0.164f));

    // TODO: Make a metal shader for this
    Resources::MATERIALS.ROBOT_METAL
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->setParam("objectColor", glm::vec3(0.380f, 0.345f, 0.321f));

    // TODO: Make a metal shader for this
    Resources::MATERIALS.ROBOT_METAL_2
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->setParam("objectColor", glm::vec3(0.360f, 0.325f, 0.301f));

    Resources::MATERIALS.ROBOT_WIRE
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->setParam("objectColor", glm::vec3(0.0f, 0.0f, 0.0f));

    Resources::MATERIALS.ROBOT_METAL_PAINTED
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->setParam("objectColor", glm::vec3(0.870f, 0.113f, 0.007f));
}

///////////////////////////////////////////////////////////////////////////
//------------------------------ MODELS ---------------------------------//
///////////////////////////////////////////////////////////////////////////
void Resources::initModels() {

    LOGE("Loading models...");

    // Primitives
    ResourceLoader::loadModelExternal("assets/models/primitives/plane.dae", Resources::MODELS.PLANE);
    ResourceLoader::loadModelExternal("assets/models/primitives/cube.dae", Resources::MODELS.CUBE);
    ResourceLoader::loadModelExternal("assets/models/primitives/complex_cube.dae", Resources::MODELS.COMPLEX_CUBE);


    // Robot
    ResourceLoader::loadModelExternal("assets/models/robot.dae", Resources::MODELS.ROBOT);

}

///////////////////////////////////////////////////////////////////////////
//----------------------------- SHADERS ---------------------------------//
///////////////////////////////////////////////////////////////////////////
void Resources::initShaders() {
}
