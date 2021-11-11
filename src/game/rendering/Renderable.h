#ifndef VULKAN_SRC_GAME_RENDERING_RENDERABLE_H
#define VULKAN_SRC_GAME_RENDERING_RENDERABLE_H

#include "game/rendering/meshes/Mesh.h"
#include "game/rendering/meshes/MeshRenderer.h"

class Renderable {
 public:
  virtual ~Renderable() = default;

  virtual const Mesh& GetMesh() const = 0;

  virtual void Render(const MeshRenderer& renderer) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RENDERABLE_H