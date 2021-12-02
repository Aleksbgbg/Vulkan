#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_MAINPLAYER_H_
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_MAINPLAYER_H_

#include "PlayerController.h"
#include "SpaceshipMesh.h"
#include "game/Actor.h"
#include "game/Camera.h"
#include "game/rendering/meshes/Mesh.h"
#include "game/renders/ParticleStream.h"
#include "game/renders/spaceships/SpaceshipModel.h"

class MainPlayer : public Actor {
 public:
  MainPlayer(SpaceshipModel spaceshipModel, Camera& camera,
             const PlayerController& playerController);

  void UpdateModel(const UpdateContext& context) override;

  const Mesh& GetMesh() const override;

  void Render(const MeshRenderer& renderer) const override;

 private:
  SpaceshipModel spaceshipModel_;
  Camera& camera_;
  const PlayerController& playerController_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_MAINPLAYER_H_
