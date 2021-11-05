#ifndef VULKAN_SRC_GAME_PLAYER_H
#define VULKAN_SRC_GAME_PLAYER_H

#include "game/Actor.h"
#include "game/Camera.h"
#include "game/rendering/resources/Mesh.h"
#include "game/renders/spaceships/SpaceshipModel.h"
#include "general/windowing/Window.h"

class Player : public Actor {
 public:
  Player(SpaceshipMesh mesh, Camera& camera, const Window& window);

  void UpdateModel(const UpdateContext& context) override;

  void WriteTexture(TextureRegistry& textureRegistry) const override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  SpaceshipModel spaceshipModel;
  Camera& camera;
  const Window& window;
};

#endif  // VULKAN_SRC_GAME_PLAYER_H
