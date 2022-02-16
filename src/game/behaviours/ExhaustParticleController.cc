#include "ExhaustParticleController.h"

ExhaustParticleController::ExhaustParticleController(Transform& parentTransform,
                                                     Visibility& visibility)
    : parentTransform_(parentTransform),
      visibility_(visibility),
      previousParentPosition_() {}

void ExhaustParticleController::UpdateModel(const UpdateContext& context) {
  const glm::vec3 parentPosition = parentTransform_.GetPosition();
  visibility_.SetIsVisible(previousParentPosition_ != parentPosition);
  previousParentPosition_ = parentPosition;
}
