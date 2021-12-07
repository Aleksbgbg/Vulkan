#include "CameraController.h"

CameraController::CameraController(game::Camera& camera,
                                   Transform& parentTransform)
    : camera_(camera), parentTransform_(parentTransform) {}

void CameraController::UpdateModel(const UpdateContext& context) {
  camera_.SetView(
      {.positionTransform = parentTransform_.GetTransform(),
       .lookAround = context.controls.NormalizedAxisValue(Axis::View),
       .reverse = context.controls.IsControlActive(Control::ReverseView)});
}
