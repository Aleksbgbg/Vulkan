#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLES_MULTIPARTICLEMESH_H_
#define VULKAN_SRC_GAME_RENDERS_PARTICLES_MULTIPARTICLEMESH_H_

#include "BufferWithMemory.h"
#include "game/rendering/meshes/Mesh.h"

class MultiParticleMesh {
 public:
  class SubMesh : public Mesh {
   public:
    SubMesh(const BufferWithMemory& vertexBuffer_,
            const BufferWithMemory& indexBuffer_, const u32 indexCount_,
            const u32 instanceCount_);

    void BindTexture(ResourceBinder& textureRegistry) const override;
    void Render(const CommandBuffer& commandBuffer,
                const PipelineLayout& pipelineLayout) const override;

   private:
    const BufferWithMemory& vertexBuffer_;
    const BufferWithMemory& indexBuffer_;
    const u32 indexCount_;
    const u32 instanceCount_;
  };

  MultiParticleMesh() = default;
  MultiParticleMesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
                    const u32 indexCount);

  SubMesh RenderInstances(const u32 instanceCount) const;

 private:
  BufferWithMemory vertexBuffer_;
  BufferWithMemory indexBuffer_;
  u32 indexCount_;
  u32 instanceCount_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLES_MULTIPARTICLEMESH_H_
