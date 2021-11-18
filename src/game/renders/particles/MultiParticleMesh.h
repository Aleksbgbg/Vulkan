#ifndef VULKAN_SRC_GAME_RENDERS_PARTICLES_MULTIPARTICLEMESH_H
#define VULKAN_SRC_GAME_RENDERS_PARTICLES_MULTIPARTICLEMESH_H

#include "BufferWithMemory.h"
#include "game/rendering/meshes/Mesh.h"

class MultiParticleMesh : public Mesh {
 public:
  MultiParticleMesh() = default;
  MultiParticleMesh(BufferWithMemory vertexBuffer, BufferWithMemory indexBuffer,
                    const u32 indexCount, const u32 instanceCount);

  void BindTexture(ResourceBinder& textureRegistry) const override;
  void Render(const CommandBuffer& commandBuffer,
              const PipelineLayout& pipelineLayout) const override;

 private:
  BufferWithMemory vertexBuffer;
  BufferWithMemory indexBuffer;
  u32 indexCount;
  u32 instanceCount;
};

#endif  // VULKAN_SRC_GAME_RENDERS_PARTICLES_MULTIPARTICLEMESH_H
