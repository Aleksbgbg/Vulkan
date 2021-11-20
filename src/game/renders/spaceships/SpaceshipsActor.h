#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPSACTOR_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPSACTOR_H

#include "NoMovementPlayerController.h"
#include "PlayerSpawnConsumer.h"
#include "SpaceshipMesh.h"
#include "SystemInputPlayerController.h"
#include "game/Actor.h"
#include "game/Camera.h"
#include "game/rendering/meshes/Mesh.h"
#include "game/renders/ParticleController.h"
#include "game/renders/spaceships/SpaceshipModel.h"
#include "general/windowing/Window.h"

class SpaceshipsActor : public Actor, public PlayerSpawnConsumer {
 public:
  SpaceshipsActor(SpaceshipMesh mesh, Camera& camera,
                  ParticleController& particleController,
                  const PlayerController& mainPlayerController);

  void UpdateModel(const UpdateContext& context) override;
  const Mesh& GetMesh() const override;
  void Render(const MeshRenderer& renderer) const override;

  void SpawnPlayer(const PlayerController& controller) override;

 private:
  bool mainPlayerSpawned_;
  SpaceshipMesh mesh_;
  Camera& camera_;
  ParticleController& particleController_;
  std::vector<std::unique_ptr<Actor>> spaceships_;
  const PlayerController& mainPlayerController_;
  SystemInputPlayerController systemInputPlayerController_;
  NoMovementPlayerController noMovementPlayerController_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPSACTOR_H
