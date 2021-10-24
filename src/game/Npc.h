#ifndef VULKAN_SRC_GAME_NPC_H
#define VULKAN_SRC_GAME_NPC_H

#include "Actor.h"
#include "SpaceshipModel.h"
#include "game/rendering/resources/Mesh.h"

class Npc : public Actor {
 public:
  Npc() = default;
  Npc(Mesh mesh);

  void UpdateModel(const UpdateContext& context) override;
  glm::vec3* Position() override;
  glm::vec3 Size() const override;
  void WriteTexture(TextureRegistry& textureRegistry) const override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  SpaceshipModel spaceshipModel;
};

#endif  // VULKAN_SRC_GAME_NPC_H
