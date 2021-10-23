#ifndef VULKAN_SRC_GAME_SPACESHIPMODEL_H
#define VULKAN_SRC_GAME_SPACESHIPMODEL_H

#include "game/Actor.h"
#include "game/rendering/resources/MeshLoader.h"
#include "util/include/glm.h"

class SpaceshipModel : public Actor {
 public:
  SpaceshipModel() = default;
  SpaceshipModel(const MeshLoader& meshLoader);

  glm::vec3* Position() override;
  glm::vec3 Size() const override;

  void WriteTexture(TextureRegistry& textureRegistry) const override;

  void UpdateModel(const UpdateContext& context) override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  Mesh mesh;
  glm::vec3 position;
  glm::mat4 transform;
};

#endif  // VULKAN_SRC_GAME_SPACESHIPMODEL_H
