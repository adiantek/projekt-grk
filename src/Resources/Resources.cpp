#include <iostream>
#include <string>

#include <Logger.h>
#include <ResourceLoader.hpp>
#include <Resources/Resources.hpp>
#include <Resources/Material.hpp>

void Resources::init() {
    Resources::initMaterials();
}

///////////////////////////////////////////////////////////////////////////
//---------------------------- MATERIALS --------------------------------//
///////////////////////////////////////////////////////////////////////////
void Resources::initMaterials() {
    LOGI("Initializing materials...");

    Resources::MATERIALS.DEFAULT
        ->setProgram(&resourceLoaderExternal->p_shader_color)
        ->withModelViewProjectionMatrix(&resourceLoaderExternal->p_shader_color_uni_modelMatrix)
        ->withModelViewProjectionMatrix(&resourceLoaderExternal->p_shader_color_uni_modelViewProjectionMatrix)
        ->withLightPosition(&resourceLoaderExternal->p_shader_color_uni_lightDir)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(0.827f, 0.827f, 0.827f));

    Resources::MATERIALS.DEFAULT_WHITE
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(1.0f, 1.0f, 1.0f));

    Resources::MATERIALS.DEFAULT_RED
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(1.0f, 0.0f, 0.0f));

    Resources::MATERIALS.DEFAULT_GREEN
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(0.0f, 1.0f, 0.0f));
    
    Resources::MATERIALS.DEFAULT_BLUE
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(0.0f, 0.0f, 1.0f));

    Resources::MATERIALS.DEFAULT_YELLOW
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(1.0f, 1.0f, 0.0f));
    
    Resources::MATERIALS.DEFAULT_CYAN
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(0.0f, 1.0f, 1.0f));
    
    Resources::MATERIALS.DEFAULT_MAGENTA
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(1.0f, 0.0f, 1.0f));

    Resources::MATERIALS.DEFAULT_BLACK
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(0.0f, 0.0f, 0.0f));
    
    Resources::MATERIALS.DEFAULT_GREY
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(0.5f, 0.5f, 0.5f));

    Resources::MATERIALS.DEFAULT_ORANGE
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(1.0f, 0.5f, 0.0f));

    Resources::MATERIALS.DEFAULT_PURPLE
        ->extend(Resources::MATERIALS.DEFAULT)
        ->setParam(&resourceLoaderExternal->p_shader_color_uni_objectColor, glm::vec3(0.5f, 0.0f, 1.0f));

    Resources::MATERIALS.ARMATURE_PBR
        ->setProgram(&resourceLoaderExternal->p_color_armature)
        ->withWaterHeight(&resourceLoaderExternal->p_color_armature_uni_waterHeight)
        ->withCameraPosition(&resourceLoaderExternal->p_color_armature_uni_cameraPosition)
        ->withLightPosition(&resourceLoaderExternal->p_color_armature_uni_lightPosition)
        ->withJointTransforms(&resourceLoaderExternal->p_color_armature_uni_jointTransforms)
        ->withLightTransformation(&resourceLoaderExternal->p_color_armature_uni_lightTransformation)
        ->withModelMatrix(&resourceLoaderExternal->p_color_armature_uni_modelMatrix)
        ->withModelViewProjectionMatrix(&resourceLoaderExternal->p_color_armature_uni_modelViewProjectionMatrix);

    // Robot materials
    Resources::MATERIALS.ROBOT_BODY
        ->extend(Resources::MATERIALS.ARMATURE_PBR)
        ->setAlbedo(&resourceLoaderExternal->p_color_armature_uni_albedoTexture, &resourceLoaderExternal->tex_entities_robot_robot_body_Albedo)
        ->setNormal(&resourceLoaderExternal->p_color_armature_uni_normalTexture, &resourceLoaderExternal->tex_entities_robot_robot_body_Normal)
        ->setAO(&resourceLoaderExternal->p_color_armature_uni_aoTexture, &resourceLoaderExternal->tex_entities_robot_robot_body_AO)
        ->setMetallic(&resourceLoaderExternal->p_color_armature_uni_metallicTexture, &resourceLoaderExternal->tex_entities_robot_robot_body_Metalness)
        ->setRoughness(&resourceLoaderExternal->p_color_armature_uni_roughnessTexture, &resourceLoaderExternal->tex_entities_robot_robot_body_Roughness);

    Resources::MATERIALS.ROBOT_CHROME
        ->extend(Resources::MATERIALS.ARMATURE_PBR)
        ->setAlbedo(&resourceLoaderExternal->p_color_armature_uni_albedoTexture, &resourceLoaderExternal->tex_entities_robot_robot_chrome_Albedo)
        ->setNormal(&resourceLoaderExternal->p_color_armature_uni_normalTexture, &resourceLoaderExternal->tex_entities_robot_robot_chrome_Normal)
        ->setAO(&resourceLoaderExternal->p_color_armature_uni_aoTexture, &resourceLoaderExternal->tex_entities_robot_robot_chrome_AO)
        ->setMetallic(&resourceLoaderExternal->p_color_armature_uni_metallicTexture, &resourceLoaderExternal->tex_entities_robot_robot_chrome_Metalness)
        ->setRoughness(&resourceLoaderExternal->p_color_armature_uni_roughnessTexture, &resourceLoaderExternal->tex_entities_robot_robot_chrome_Roughness);

    Resources::MATERIALS.ROBOT_GLOW
        ->extend(Resources::MATERIALS.ARMATURE_PBR)
        ->setAlbedo(&resourceLoaderExternal->p_color_armature_uni_albedoTexture, &resourceLoaderExternal->tex_entities_robot_robot_glow_Albedo)
        ->setNormal(&resourceLoaderExternal->p_color_armature_uni_normalTexture, &resourceLoaderExternal->tex_entities_robot_robot_glow_Normal)
        ->setAO(&resourceLoaderExternal->p_color_armature_uni_aoTexture, &resourceLoaderExternal->tex_entities_robot_robot_glow_AO)
        ->setMetallic(&resourceLoaderExternal->p_color_armature_uni_metallicTexture, &resourceLoaderExternal->tex_entities_robot_robot_glow_Metalness)
        ->setRoughness(&resourceLoaderExternal->p_color_armature_uni_roughnessTexture, &resourceLoaderExternal->tex_entities_robot_robot_glow_Roughness);

    Resources::MATERIALS.ROBOT_LENS
        ->extend(Resources::MATERIALS.ARMATURE_PBR)
        ->setAlbedo(&resourceLoaderExternal->p_color_armature_uni_albedoTexture, &resourceLoaderExternal->tex_entities_robot_robot_lens_Albedo)
        ->setNormal(&resourceLoaderExternal->p_color_armature_uni_normalTexture, &resourceLoaderExternal->tex_entities_robot_robot_lens_Normal)
        ->setAO(&resourceLoaderExternal->p_color_armature_uni_aoTexture, &resourceLoaderExternal->tex_entities_robot_robot_lens_AO)
        ->setMetallic(&resourceLoaderExternal->p_color_armature_uni_metallicTexture, &resourceLoaderExternal->tex_entities_robot_robot_lens_Metalness)
        ->setRoughness(&resourceLoaderExternal->p_color_armature_uni_roughnessTexture, &resourceLoaderExternal->tex_entities_robot_robot_lens_Roughness);

    Resources::MATERIALS.ROBOT_METAL
        ->extend(Resources::MATERIALS.ARMATURE_PBR)
        ->setAlbedo(&resourceLoaderExternal->p_color_armature_uni_albedoTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_Albedo)
        ->setNormal(&resourceLoaderExternal->p_color_armature_uni_normalTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_Normal)
        ->setAO(&resourceLoaderExternal->p_color_armature_uni_aoTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_AO)
        ->setMetallic(&resourceLoaderExternal->p_color_armature_uni_metallicTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_Metalness)
        ->setRoughness(&resourceLoaderExternal->p_color_armature_uni_roughnessTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_Roughness);

    Resources::MATERIALS.ROBOT_METAL_2
        ->extend(Resources::MATERIALS.ARMATURE_PBR)
        ->setAlbedo(&resourceLoaderExternal->p_color_armature_uni_albedoTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_2_Albedo)
        ->setNormal(&resourceLoaderExternal->p_color_armature_uni_normalTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_2_Normal)
        ->setAO(&resourceLoaderExternal->p_color_armature_uni_aoTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_2_AO)
        ->setMetallic(&resourceLoaderExternal->p_color_armature_uni_metallicTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_2_Metalness)
        ->setRoughness(&resourceLoaderExternal->p_color_armature_uni_roughnessTexture, &resourceLoaderExternal->tex_entities_robot_robot_metal_2_Roughness);

    Resources::MATERIALS.ROBOT_WIRE
        ->extend(Resources::MATERIALS.ARMATURE_PBR)
        ->setAlbedo(&resourceLoaderExternal->p_color_armature_uni_albedoTexture, &resourceLoaderExternal->tex_entities_robot_robot_wire_Albedo)
        ->setNormal(&resourceLoaderExternal->p_color_armature_uni_normalTexture, &resourceLoaderExternal->tex_entities_robot_robot_wire_Normal)
        ->setAO(&resourceLoaderExternal->p_color_armature_uni_aoTexture, &resourceLoaderExternal->tex_entities_robot_robot_wire_AO)
        ->setMetallic(&resourceLoaderExternal->p_color_armature_uni_metallicTexture, &resourceLoaderExternal->tex_entities_robot_robot_wire_Metalness)
        ->setRoughness(&resourceLoaderExternal->p_color_armature_uni_roughnessTexture, &resourceLoaderExternal->tex_entities_robot_robot_wire_Roughness);

    Resources::MATERIALS.ROBOT_PROPELERS
        ->extend(Resources::MATERIALS.ARMATURE_PBR)
        ->setAlbedo(&resourceLoaderExternal->p_color_armature_uni_albedoTexture, &resourceLoaderExternal->tex_entities_robot_robot_propelers_Albedo)
        ->setNormal(&resourceLoaderExternal->p_color_armature_uni_normalTexture, &resourceLoaderExternal->tex_entities_robot_robot_propelers_Normal)
        ->setAO(&resourceLoaderExternal->p_color_armature_uni_aoTexture, &resourceLoaderExternal->tex_entities_robot_robot_propelers_AO)
        ->setMetallic(&resourceLoaderExternal->p_color_armature_uni_metallicTexture, &resourceLoaderExternal->tex_entities_robot_robot_propelers_Metalness)
        ->setRoughness(&resourceLoaderExternal->p_color_armature_uni_roughnessTexture, &resourceLoaderExternal->tex_entities_robot_robot_propelers_Roughness);
        
}
