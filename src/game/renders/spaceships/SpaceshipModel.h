#ifndef VULKAN_SRC_GAME_SPACESHIPMODEL_H
#define VULKAN_SRC_GAME_SPACESHIPMODEL_H

#include "SpaceshipMesh.h"
#include "game/Actor.h"
#include "game/renders/ParticleStream.h"
#include "util/include/glm.h"

class SpaceshipModel {
 public:
  SpaceshipModel(SpaceshipMesh& mesh, ParticleStream& exhaustParticles);

  glm::vec3* Position();
  glm::vec3 Size() const;

  const Mesh& GetMesh() const;

  void Rotate(const glm::vec3 rotation);
  void Move(const glm::vec3 movement, const float deltaTime);

  void Render(const MeshRenderer& renderer) const;

 private:
  SpaceshipMesh& mesh_;
  ParticleStream& exhaustParticles_;
  glm::vec3 position_;
  glm::vec3 rotation_;
};

#endif  // VULKAN_SRC_GAME_SPACESHIPMODEL_H
