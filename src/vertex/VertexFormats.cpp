#include <vertex/VertexFormats.hpp>

using namespace vertex;

VertexFormat::VertexFormat() {
}

void VertexFormat::addPos() {
    this->pos = this->size;
    this->size += 12;  // sizeof(vec3)
}

void VertexFormat::addColor() {
    this->color = this->size;
    this->size += 4 * 4;  // sizeof(vec4)
}

void VertexFormat::addTex() {
    this->tex = this->size;
    this->size += 8;  // sizeof(vec2)
}

void VertexFormat::addNormal() {
    this->normal = this->size;
    this->size += 12;  // sizeof(vec3)
}

void VertexFormat::addTangent() {
    this->tangent = this->size;
    this->size += 12;  // sizeof(vec3)
}

void VertexFormat::addBitangent() {
    this->bitangent = this->size;
    this->size += 12;  // sizeof(vec3)
}

void VertexFormat::addJoint() {
    this->jointID = this->size;
    this->size += 3 * 4;
    this->jointWeight = this->size;
    this->size += 3 * 4;
}

int32_t VertexFormat::getGPUSize() {
    // padding - better performance
    return (this->size + 3) / 4 * 4;
}

VertexFormat vertex::POS;
VertexFormat vertex::POS_COLOR;
VertexFormat vertex::POS_TEX;
VertexFormat vertex::POS_NORMAL_TEX;
VertexFormat vertex::POS_NORMAL_TEX_TANGENT_BITANGENT;
VertexFormat vertex::POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT;

void vertex::VertexFormats_load() {
    POS.addPos();

    POS_COLOR.addPos();
    POS_COLOR.addColor();

    POS_TEX.addPos();
    POS_TEX.addTex();

    POS_NORMAL_TEX.addPos();
    POS_NORMAL_TEX.addNormal();
    POS_NORMAL_TEX.addTex();

    POS_NORMAL_TEX_TANGENT_BITANGENT.addPos();
    POS_NORMAL_TEX_TANGENT_BITANGENT.addNormal();
    POS_NORMAL_TEX_TANGENT_BITANGENT.addTex();
    POS_NORMAL_TEX_TANGENT_BITANGENT.addTangent();
    POS_NORMAL_TEX_TANGENT_BITANGENT.addBitangent();

    POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT.addPos();
    POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT.addNormal();
    POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT.addTex();
    POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT.addTangent();
    POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT.addBitangent();
    POS_NORMAL_TEX_TANGENT_BITANGENT_JOINT.addJoint();
}
