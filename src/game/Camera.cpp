#include "Camera.h"

Camera::View Camera::GetView() const {
  return view;
}

void Camera::SetView(const Camera::View& value) {
  view = value;
}
