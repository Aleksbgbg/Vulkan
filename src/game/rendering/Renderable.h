#ifndef VULKAN_SRC_GAME_RENDERING_RENDERABLE_H
#define VULKAN_SRC_GAME_RENDERING_RENDERABLE_H

#include "game/rendering/resources/MeshRenderer.h"
#include "game/rendering/resources/TextureRegistry.h"

class Renderable {
 public:
  virtual ~Renderable() = default;

  virtual void WriteTexture(TextureRegistry& textureRegistry) const = 0;
  virtual void Render(const MeshRenderer& renderer) const = 0;
};

#endif  // VULKAN_SRC_GAME_RENDERING_RENDERABLE_H
