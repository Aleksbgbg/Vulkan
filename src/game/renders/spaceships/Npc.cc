#include "Npc.h"

Npc::Npc(SpaceshipMesh mesh, ParticleStream& exhaustParticles)
    : mesh_(std::move(mesh)), spaceshipModel_(mesh_, exhaustParticles) {
  spaceshipModel_.Move(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
}

void Npc::UpdateModel(const UpdateContext& context) {
  spaceshipModel_.Move(
      glm::vec3(
          0.0f, 0.0f,
          context.controls.IsControlActive(Control::MoveNpc) ? 1.0f : 0.0f),
      context.deltaTime);
}

const Mesh& Npc::GetMesh() const {
  return mesh_;
}

void Npc::Render(const MeshRenderer& renderer) const {
  spaceshipModel_.Render(renderer);
}
