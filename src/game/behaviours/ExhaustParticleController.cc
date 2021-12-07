#include "ExhaustParticleController.h"

ExhaustParticleController::ExhaustParticleController(
    Transform& parentTransform, ParticleController& particleController)
    : parentTransform_(parentTransform),
      particleController_(particleController),
      previousParentPosition_() {}

void ExhaustParticleController::UpdateModel(const UpdateContext& context) {
  const glm::vec3 parentPosition = parentTransform_.GetPosition();
  particleController_.EnableParticles(previousParentPosition_ !=
                                      parentPosition);
  previousParentPosition_ = parentPosition;
}
