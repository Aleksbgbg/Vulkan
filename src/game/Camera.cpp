#include "Camera.h"

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(view.position, view.lookAt, Up());
}

glm::vec3 Camera::GetPosition() const {
  return view.position;
}

void Camera::SetView(const Camera::View& value) {
  view = value;
}

glm::vec3 Camera::Up() const {
  const glm::vec3 viewDirection =
      glm::normalize(view.reverse ? (view.lookAt - view.position)
                                  : (view.position - view.lookAt));
  const glm::vec3 forward =
      glm::toMat4(glm::quat(glm::vec3(0.0f, view.rotation.y, 0.0f))) *
      glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
  const float angle = std::acos(glm::dot(viewDirection, forward));

  const bool isUpsideDown = angle <= QuarterTurn;

  if (isUpsideDown) {
    return glm::vec3(0.0f, -1.0f, 0.0f);
  }

  return glm::vec3(0.0f, 1.0f, 0.0f);
}
