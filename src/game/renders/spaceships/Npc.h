#ifndef VULKAN_SRC_GAME_NPC_H
#define VULKAN_SRC_GAME_NPC_H

#include "game/Actor.h"
#include "game/rendering/resources/Mesh.h"
#include "game/renders/spaceships/SpaceshipModel.h"

class Npc : public Actor {
 public:
  Npc() = default;
  Npc(SpaceshipMesh mesh);

  void UpdateModel(const UpdateContext& context) override;

  void WriteTexture(TextureRegistry& textureRegistry) const;
  void Render(const MeshRenderer& renderer) const override;

 private:
  SpaceshipModel spaceshipModel;
};

#endif  // VULKAN_SRC_GAME_NPC_H
