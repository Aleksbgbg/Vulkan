#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_OTHERPLAYER_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_OTHERPLAYER_H

#include "PlayerController.h"
#include "SpaceshipMesh.h"
#include "game/Actor.h"
#include "game/rendering/meshes/Mesh.h"
#include "game/renders/ParticleStream.h"
#include "game/renders/spaceships/SpaceshipModel.h"

class OtherPlayer : public Actor {
 public:
  OtherPlayer(SpaceshipModel spaceshipModel,
              const PlayerController& playerController);

  void UpdateModel(const UpdateContext& context) override;

  const Mesh& GetMesh() const override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  SpaceshipModel spaceshipModel_;
  const PlayerController& playerController_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_OTHERPLAYER_H
