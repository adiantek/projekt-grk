#include <Logger.h>

#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <world/ChunkBorderDebugRenderer.hpp>

using namespace world;

ChunkBorderDebugRenderer::ChunkBorderDebugRenderer() {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    this->vb = new vertex::VertexBuffer(&vertex::POS_COLOR, 976 + 144);
}

ChunkBorderDebugRenderer::~ChunkBorderDebugRenderer() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
    delete this->vb;
}

void ChunkBorderDebugRenderer::update() {
    ChunkPosition pos(robot->position);
    if (this->currCords.x != pos.coords.x || this->currCords.z != pos.coords.z) {
        this->prepared = false;
    }
    if (this->prepared) {
        return;
    }
    LOGD("position: %.3f %.3f %.3f, chunk: %d %d", robot->position.x, robot->position.y, robot->position.z, pos.coords.x, pos.coords.z);
    this->prepared = true;

    this->currCords.x = pos.coords.x;
    this->currCords.z = pos.coords.z;

    this->vb->clear();

    float minY = 0;
    float maxY = 256;

    float cx = (float)(pos.coords.x << 4);
    float cz = (float)(pos.coords.z << 4);

    // glowne granice chunka
    for (int x = -16; x <= 32; x += 16) {
        for (int z = -16; z <= 32; z += 16) {
            if ((x == 0 || x == 16) && (z == 0 || z == 16)) {
                // skip dla granic sekcji
                continue;
            }
            this->vb->pos(cx + (float)x, minY, cz + (float)z)->color(1.0F, 0.0F, 0.0F, 0.5F)->end();
            this->vb->pos(cx + (float)x, maxY, cz + (float)z)->color(1.0F, 0.0F, 0.0F, 0.5F)->end();
        }
    }

    // scianki po dwie jednostki "na stojaco"
    for (int x = 2; x < 16; x += 2) {
        this->vb->pos(cx + (float)x, minY, cz)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
        this->vb->pos(cx + (float)x, maxY, cz)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();

        this->vb->pos(cx + (float)x, minY, cz + 16.0F)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
        this->vb->pos(cx + (float)x, maxY, cz + 16.0F)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
    }

    for (int z = 2; z < 16; z += 2) {
        this->vb->pos(cx, minY, cz + (float)z)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
        this->vb->pos(cx, maxY, cz + (float)z)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();

        this->vb->pos(cx + 16.0F, minY, cz + (float)z)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
        this->vb->pos(cx + 16.0F, maxY, cz + (float)z)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
    }

    // scianki po dwie jednostki "na lezaco"
    for (int y = 0; y <= 256; y += 2) {
        if ((y & 0xF) == 0) {
            // skipujemy, aby granice sekcji byly wyrazniejsze
            continue;
        }
        float fy = (float)y;

        this->vb->pos(cx, fy, cz)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
        this->vb->pos(cx, fy, cz + 16.0F)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();

        this->vb->pos(cx, fy, cz + 16.0F)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
        this->vb->pos(cx + 16.0F, fy, cz + 16.0F)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();

        this->vb->pos(cx + 16.0F, fy, cz + 16.0F)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
        this->vb->pos(cx + 16.0F, fy, cz)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();

        this->vb->pos(cx + 16.0F, fy, cz)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
        this->vb->pos(cx, fy, cz)->color(1.0F, 1.0F, 0.0F, 1.0F)->end();
    }
    this->sizeOne = this->vb->getVertices();
    // granice sekcji "na stojaco"
    for (int x = 0; x <= 16; x += 16) {
        for (int z = 0; z <= 16; z += 16) {
            this->vb->pos(cx + (float)x, minY, cz + (float)z)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();
            this->vb->pos(cx + (float)x, maxY, cz + (float)z)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();
        }
    }

    // granice sekcji "na lezaco"
    for (int y = 0; y <= 256; y += 16) {
        float fy = (float)y;

        this->vb->pos(cx, fy, cz)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();
        this->vb->pos(cx, fy, cz + 16.0F)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();

        this->vb->pos(cx, fy, cz + 16.0F)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();
        this->vb->pos(cx + 16.0F, fy, cz + 16.0F)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();

        this->vb->pos(cx + 16.0F, fy, cz + 16.0F)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();
        this->vb->pos(cx + 16.0F, fy, cz)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();

        this->vb->pos(cx + 16.0F, fy, cz)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();
        this->vb->pos(cx, fy, cz)->color(0.25F, 0.25F, 1.0F, 1.0F)->end();
    }

    glUseProgram(resourceLoaderExternal->p_simple_color_shader);
    glBindVertexArray(this->vao);
    this->vb->updateVBO(this->vbo);
    this->vb->configureColor(resourceLoaderExternal->p_simple_color_shader_attr_vertexColor);
    this->vb->configurePos(resourceLoaderExternal->p_simple_color_shader_attr_vertexPosition);

    // LOGD("vertices: %d + %d", this->sizeOne, this->vb->getVertices() - this->sizeOne);
}

void ChunkBorderDebugRenderer::draw(glm::mat4 mat) {
    glUseProgram(resourceLoaderExternal->p_simple_color_shader);
    glUniformMatrix4fv(resourceLoaderExternal->p_simple_color_shader_uni_transformation, 1, GL_FALSE, glm::value_ptr(mat));
    glBindVertexArray(this->vao);
    glLineWidth(1.0F);
    glDrawArrays(GL_LINES, 0, this->sizeOne);
    glLineWidth(1.0F);
    glDrawArrays(GL_LINES, this->sizeOne, this->vb->getVertices() - this->sizeOne);
}