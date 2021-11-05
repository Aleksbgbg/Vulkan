#ifndef VULKAN_SRC_GAME_LIGHTBOX_H
#define VULKAN_SRC_GAME_LIGHTBOX_H

#include "LightMesh.h"
#include "game/Actor.h"
#include "game/rendering/resources/MeshRenderer.h"
#include "game/rendering/resources/ResourceLoader.h"

class LightBox : public Actor {
 public:
  LightBox() = default;
  LightBox(const ResourceLoader& resourceLoader);

  void UpdateModel(const UpdateContext& context) override;

  void WriteTexture(TextureRegistry& textureRegistry) const override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  LightMesh lightMesh;
};

#endif  // VULKAN_SRC_GAME_LIGHTBOX_H
