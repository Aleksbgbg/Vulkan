#include "Camera.h"

namespace game {

Camera::Camera() : cameraTransform_(1.0f) {}

glm::mat4 Camera::GetViewMatrix() const {
  return cameraTransform_;
}

// Every transformation here is applied to the world to put the camera in the
// center, so it should be the reverse of where you want the camera to be!
void Camera::SetView(const Camera::View& view) {
  const glm::mat4 positionTranslation =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -5.5f));

  glm::quat lookAround = Rotation(QUARTER_TURN * view.lookAround.y, X_AXIS) *
                         Rotation(QUARTER_TURN * view.lookAround.x, Y_AXIS);
  // Camera looks down negative z-axis by default, which is looking in reverse
  // When not in reverse, rotate to look down positive z-axis
  if (!view.reverse) {
    lookAround *= Rotation(HALF_TURN, Y_AXIS);
  }
  const glm::mat4 lookAroundRotation = glm::toMat4(lookAround);

  const glm::mat4 moveToAttachedObjectTransformation =
      glm::inverse(view.positionTransform);

  cameraTransform_ = positionTranslation * lookAroundRotation *
                     moveToAttachedObjectTransformation;
}

}  // namespace game
