#ifndef VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERSPACESHIPS_H
#define VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERSPACESHIPS_H

#include "SpaceshipMesh.h"
#include "game/Actor.h"
#include "game/Camera.h"
#include "game/rendering/meshes/Mesh.h"
#include "game/renders/ParticleController.h"
#include "game/renders/spaceships/SpaceshipModel.h"
#include "general/windowing/Window.h"

class PlayerSpaceships : public Actor {
 public:
  PlayerSpaceships(SpaceshipMesh mesh, Camera& camera,
                   const wnd::Window& window,
                   ParticleController& particleController);

  void UpdateModel(const UpdateContext& context) override;
  const Mesh& GetMesh() const override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  SpaceshipMesh mesh_;
  Camera& camera_;
  const wnd::Window& window_;
  ParticleController& particleController_;
  std::vector<std::unique_ptr<Actor>> spaceships_;
};

#endif  // VULKAN_SRC_GAME_RENDERS_SPACESHIPS_PLAYERSPACESHIPS_H
