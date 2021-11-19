#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPSACTOR_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPSACTOR_H

#include "NoMovementPlayerController.h"
#include "SpaceshipMesh.h"
#include "SystemInputPlayerController.h"
#include "game/Actor.h"
#include "game/Camera.h"
#include "game/rendering/meshes/Mesh.h"
#include "game/renders/ParticleController.h"
#include "game/renders/spaceships/SpaceshipModel.h"
#include "general/windowing/Window.h"

class SpaceshipsActor : public Actor {
 public:
  SpaceshipsActor(SpaceshipMesh mesh, Camera& camera,
                  ParticleController& particleController);

  void UpdateModel(const UpdateContext& context) override;
  const Mesh& GetMesh() const override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  SpaceshipMesh mesh_;
  Camera& camera_;
  ParticleController& particleController_;
  std::vector<std::unique_ptr<Actor>> spaceships_;
  SystemInputPlayerController systemInputPlayerController_;
  NoMovementPlayerController noMovementPlayerController_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_SPACESHIPSACTOR_H
