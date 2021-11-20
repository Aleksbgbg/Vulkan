#include "Npc.h"

Npc::Npc(SpaceshipMesh mesh, ParticleStream& exhaustParticles)
    : mesh_(std::move(mesh)), spaceshipModel_(mesh_, exhaustParticles) {
  spaceshipModel_.SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
}

void Npc::UpdateModel(const UpdateContext& context) {
  const bool isMoving = context.controls.IsControlActive(Control::MoveNpc);

  spaceshipModel_.SetIsMoving(isMoving);
  if (isMoving) {
    spaceshipModel_.SetPosition(
        *spaceshipModel_.Position() +
        glm::vec3(0.0f, 0.0f, 10.0f * context.deltaTime));
  }
  spaceshipModel_.Update();
}

const Mesh& Npc::GetMesh() const {
  return mesh_;
}

void Npc::Render(const MeshRenderer& renderer) const {
  spaceshipModel_.Render(renderer);
}
