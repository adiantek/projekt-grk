#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <Logger.h>

class Fog {
   public:
    Fog(unsigned int width, unsigned int height, float maxDepth);
    ~Fog();

    unsigned int framebuffer;
    
    void useFramebuffer();
    void stopUsingFramebuffer();
    unsigned int getMapTexture();
    void update();

   private:
    
    unsigned int depthbuffer;
    unsigned int texture;
    unsigned int depthTexture;
    
    float quadVao[24];
    float maxDepth;
    unsigned int width;
    unsigned int height;
    int prevViewport[4];

    GLuint rectVAO, rectVBO;
};
extern Fog *fog;