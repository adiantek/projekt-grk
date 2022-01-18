#pragma once

#include <stdint.h>

namespace vertex {

class VertexFormat {
   private:
    int32_t size = 0;

   public:
    VertexFormat();

    int32_t pos = -1;
    int32_t color = -1;
    int32_t tex = -1;
    int32_t normal = -1;
    int32_t tangent = -1;
    int32_t bitangent = -1;
    int32_t jointID = -1;
    int32_t jointWeight = -1;

    void addPos();
    void addColor();
    void addTex();
    void addNormal();
    void addTangent();
    void addBitangent();
    void addJoint();

    int32_t getGPUSize();
};

extern VertexFormat POS;
extern VertexFormat POS_COLOR;
extern VertexFormat POS_TEX;
extern VertexFormat POS_NORMAL_TEX;
extern VertexFormat POS_NORMAL_TEX_TANGENT_BITANGENT;
extern VertexFormat POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT;

void VertexFormats_load();

}  // namespace vertex