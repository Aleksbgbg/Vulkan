#ifndef VULKAN_SRC_GAME_LIGHTBOX_H
#define VULKAN_SRC_GAME_LIGHTBOX_H

#include "game/Actor.h"
#include "game/rendering/meshes/MeshRenderer.h"
#include "game/rendering/meshes/TexturedMesh.h"
#include "game/rendering/resources/ResourceLoader.h"

class LightBox : public Actor {
 public:
  LightBox() = default;
  LightBox(const ResourceLoader& resourceLoader);

  void UpdateModel(const UpdateContext& context) override;

  const Mesh& GetMesh() const override;

  void Render(const MeshRenderer& renderer) const override;

 private:
  TexturedMesh mesh;
};

#endif  // VULKAN_SRC_GAME_LIGHTBOX_H
