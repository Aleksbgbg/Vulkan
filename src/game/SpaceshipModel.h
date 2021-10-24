#ifndef VULKAN_SRC_GAME_SPACESHIPMODEL_H
#define VULKAN_SRC_GAME_SPACESHIPMODEL_H

#include "game/Actor.h"
#include "game/rendering/resources/Mesh.h"
#include "util/include/glm.h"

class SpaceshipModel {
 public:
  SpaceshipModel() = default;
  SpaceshipModel(Mesh mesh);

  glm::vec3* Position();
  glm::vec3 Size() const;

  void WriteTexture(TextureRegistry& textureRegistry) const;

  void Move(const glm::vec3 movement, const float deltaTime);
  void Render(const MeshRenderer& renderer) const;

 private:
  Mesh mesh;
  glm::vec3 position;
  glm::mat4 transform;
};

#endif  // VULKAN_SRC_GAME_SPACESHIPMODEL_H
