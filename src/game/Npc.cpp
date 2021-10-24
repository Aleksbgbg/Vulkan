#include "Npc.h"

Npc::Npc(Mesh mesh) : spaceshipModel(std::move(mesh)) {
  spaceshipModel.Move(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
}

void Npc::UpdateModel(const UpdateContext& context) {
  spaceshipModel.Move(
      glm::vec3(0.0f, 0.0f, context.keyboard.IsKeyDown(SDLK_f) ? 1.0f : 0.0f),
      context.deltaTime);
}

glm::vec3* Npc::Position() {
  return spaceshipModel.Position();
}

glm::vec3 Npc::Size() const {
  return spaceshipModel.Size();
}

void Npc::WriteTexture(TextureRegistry& textureRegistry) const {
  spaceshipModel.WriteTexture(textureRegistry);
}

void Npc::Render(const MeshRenderer& renderer) const {
  spaceshipModel.Render(renderer);
}
