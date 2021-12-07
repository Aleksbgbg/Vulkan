#include "Transform.h"

PropertyKey Transform::Key() {
  return PropertyKey::Transform;
}

Transform::Transform(const Transformable* parent)
    : parent_(parent),
      position_(0.0f),
      scale_(1.0f),
      rotation_(IDENTITY_ROTATION) {}

glm::vec3 Transform::GetPosition() const {
  return position_;
}

glm::mat4 Transform::GetTransform() const {
  glm::mat4 transform(1.0f);
  transform = glm::translate(transform, position_);
  transform = glm::scale(transform, scale_);
  transform = transform * glm::toMat4(rotation_);

  if (parent_ == nullptr) {
    return transform;
  }

  return transform * parent_->GetTransform();
}

void Transform::Move(const glm::vec3 position) {
  position_ = position;
}

void Transform::Scale(const glm::vec3 scale) {
  scale_ = scale;
}

void Transform::Rotate(const glm::quat rotation) {
  rotation_ = rotation;
}

void Transform::MoveBy(const glm::vec3 amount) {
  position_ += amount;
}
