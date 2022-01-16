#pragma once

#include <opengl.h>
#include <stdbool.h>

#include <Robot/Robot.hpp>
#include <vertex/VertexBuffer.hpp>
#include <world/ChunkPosition.hpp>
#include <world/Object3D.hpp>

namespace world {

class ChunkBorderDebugRenderer : Object3D {
   private:
    bool prepared = false;
    ChunkCoords currCords;
    GLuint vbo[2];
    GLuint vao[2];
    vertex::VertexBuffer *vb[2];

   public:
    ChunkBorderDebugRenderer();
    virtual ~ChunkBorderDebugRenderer();

    void update() override;
    void draw(glm::mat4 mat) override;
};

}  // namespace world