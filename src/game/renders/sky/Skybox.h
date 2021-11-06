#ifndef VULKAN_SRC_GAME_SKYBOX_H
#define VULKAN_SRC_GAME_SKYBOX_H

#include "game/Actor.h"
#include "game/rendering/meshes/TexturedMesh.h"
#include "game/rendering/resources/ResourceLoader.h"

class Skybox : public Actor {
 public:
  Skybox() = default;
  Skybox(const ResourceLoader& resourceLoader);

  void WriteTexture(TextureRegistry& textureRegistry) const;

  void UpdateModel(const UpdateContext& context) override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  TexturedMesh mesh;
};

#endif  // VULKAN_SRC_GAME_SKYBOX_H
