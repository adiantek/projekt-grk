#include <opengl.h>

#include <world/Chest.hpp>
#include <ResourceLoader.hpp>

using namespace world;

// [     8.454 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:20 ] Processing model: assets/models/props/chest.dae
// [     8.464 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:172 ] Armature_root:000001EC1BA0D850
// [     8.464 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:172 ] Armature_top:000001EC1BB20B10
// [     8.464 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:186 ] Armature_root:000001EC1BA0D850
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\Resources\Model.cpp:186 ] Armature_top:000001EC1BB20B10
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\utils\Gizmos.cpp:82 ] Dumping joints:
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\utils\Gizmos.cpp:88 ] 0: root (parent: (null), matrix: 1.00 0.00 0.00 0.00 ...)
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\utils\Gizmos.cpp:88 ] 1: Armature_root (parent: root, matrix: 1.00 0.00 0.00 0.00 ...)
// [     8.465 ] D [ G:\git\emcc\projekt-grk\src\utils\Gizmos.cpp:88 ] 2: Armature_top (parent: Armature_root, matrix: 1.00 0.00 0.00 0.00 ...)

Chest::Chest(glm::mat4 model) {
    this->model = model * glm::scale(glm::vec3(0.01f));
}

Chest::~Chest() {
}

void Chest::update() {
}

void Chest::draw(glm::mat4 mat) {
    ResourceLoader *res = resourceLoaderExternal;
    glUseProgram(res->p_simple_tex_shader);
    glUniformMatrix4fv(res->p_simple_tex_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat * this->model));
    glUniform1i(res->p_simple_tex_shader_uni_textureSampler, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, res->tex_props_chest_chest_reinforcment_albedo);
    Core::DrawContext(*(res->m_props_chest->getMeshes()[0]->getRenderContext()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, res->tex_uv_grid);
    Core::DrawContext(*(res->m_props_chest->getMeshes()[1]->getRenderContext()));
    
}

void Chest::drawShadow(glm::mat4 mat) {
}