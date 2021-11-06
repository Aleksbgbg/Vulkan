#include "Npc.h"

Npc::Npc(SpaceshipMesh mesh) : spaceshipModel(std::move(mesh)) {
  spaceshipModel.Move(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
}

void Npc::UpdateModel(const UpdateContext& context) {
  spaceshipModel.Move(
      glm::vec3(0.0f, 0.0f, context.keyboard.IsKeyDown(SDLK_f) ? 1.0f : 0.0f),
      context.deltaTime);
}

const Mesh& Npc::GetMesh() const {
  return spaceshipModel.GetMesh();
}

void Npc::Render(const MeshRenderer& renderer) const {
  spaceshipModel.Render(renderer);
}
