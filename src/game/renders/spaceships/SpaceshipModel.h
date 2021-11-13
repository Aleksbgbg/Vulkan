#ifndef VULKAN_SRC_GAME_SPACESHIPMODEL_H
#define VULKAN_SRC_GAME_SPACESHIPMODEL_H

#include "SpaceshipMesh.h"
#include "game/Actor.h"
#include "util/include/glm.h"

class SpaceshipModel {
 public:
  SpaceshipModel() = default;
  SpaceshipModel(SpaceshipMesh mesh);

  glm::vec3* Position();
  glm::vec3 Size() const;

  const Mesh& GetMesh() const;

  void Rotate(const glm::vec3 rotation);
  bool Move(const glm::vec3 movement, const float deltaTime);

  void Render(const MeshRenderer& renderer) const;

 private:
  SpaceshipMesh mesh;
  glm::vec3 position;
  glm::vec3 rotation;
};

#endif  // VULKAN_SRC_GAME_SPACESHIPMODEL_H
