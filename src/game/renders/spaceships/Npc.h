#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NPC_H_
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NPC_H_

#include "game/Actor.h"
#include "game/rendering/meshes/Mesh.h"
#include "game/renders/spaceships/SpaceshipModel.h"

class Npc : public Actor {
 public:
  Npc(SpaceshipMesh mesh, ParticleStream& exhaustParticles);

  void UpdateModel(const UpdateContext& context) override;

  const Mesh& GetMesh() const override;

  void Render(const MeshRenderer& renderer) const override;

 private:
  SpaceshipMesh mesh_;
  SpaceshipModel spaceshipModel_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_NPC_H_
