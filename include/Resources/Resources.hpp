#pragma once

#include <opengl.h>

#include <Resources/Material.hpp>
#include <Resources/Model.hpp>

class Resources {
   public:
    //////////////////////////////////////////////////////////////////////////
    //---------------------------- TEXTURES --------------------------------//
    //////////////////////////////////////////////////////////////////////////
    static struct _TEXTURES {

        // Robot textures
        inline static GLuint ROBOT_BODY_DIFFUSE = 0;
        inline static GLuint ROBOT_BODY_AO = 0;
        inline static GLuint ROBOT_BODY_NORMAL = 0;
        inline static GLuint ROBOT_BODY_ROUGHNESS = 0;

        inline static GLuint ROBOT_METAL_DIFFUSE = 0;
        inline static GLuint ROBOT_METAL_AO = 0;
        inline static GLuint ROBOT_METAL_NORMAL = 0;
        inline static GLuint ROBOT_METAL_ROUGHNESS = 0;

    } TEXTURES;

    ///////////////////////////////////////////////////////////////////////////
    //---------------------------- MATERIALS --------------------------------//
    ///////////////////////////////////////////////////////////////////////////
    static struct _MATERIALS {

        // Default materials
        inline static Material* DEFAULT = new Material();
        inline static Material* DEFAULT_WHITE = new Material();
        inline static Material* DEFAULT_RED = new Material();
        inline static Material* DEFAULT_GREEN = new Material();
        inline static Material* DEFAULT_BLUE = new Material();
        inline static Material* DEFAULT_YELLOW = new Material();
        inline static Material* DEFAULT_CYAN = new Material();
        inline static Material* DEFAULT_MAGENTA = new Material();
        inline static Material* DEFAULT_BLACK = new Material();
        inline static Material* DEFAULT_GREY = new Material();
        inline static Material* DEFAULT_ORANGE = new Material();
        inline static Material* DEFAULT_PURPLE = new Material();

        // Robot materials
        inline static Material* ROBOT_BODY = new Material();
        inline static Material* ROBOT_METAL = new Material();
        inline static Material* ROBOT_CHROME = new Material();
        inline static Material* ROBOT_LENS = new Material();
        inline static Material* ROBOT_WIRE = new Material();
        inline static Material* ROBOT_GLOW = new Material();
        inline static Material* ROBOT_METAL_2 = new Material();
        inline static Material* ROBOT_METAL_PAINTED = new Material();

    } MATERIALS;

    ///////////////////////////////////////////////////////////////////////////
    //----------------------------- SHADERS ---------------------------------//
    ///////////////////////////////////////////////////////////////////////////
    static struct _PROGRAMS {

        // Default programs
        inline static GLuint DEFAULT_PROGRAM = 0;

    } PROGRAMS;

    ///////////////////////////////////////////////////////////////////////////
    //------------------------- END OF REGISTRY -----------------------------//
    ///////////////////////////////////////////////////////////////////////////
    static void init();

   private:
    static void initTextures();
    static void initMaterials();
    static void initShaders();
    static void loadTexture();
    static void loadMaterial();
    static void loadShader();
};
