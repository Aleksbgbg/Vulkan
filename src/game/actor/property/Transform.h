#ifndef VULKAN_SRC_GAME_ACTOR_PROPERTY_TRANSFORM_H_
#define VULKAN_SRC_GAME_ACTOR_PROPERTY_TRANSFORM_H_

#include "Property.h"
#include "PropertyKey.h"
#include "game/Transformable.h"
#include "util/include/glm.h"

class Transform : public Property, public Transformable {
 public:
  static constexpr PropertyKey Key() {
    return PropertyKey::Transform;
  }

  Transform(const Transformable* parent);

  glm::vec3 GetPosition() const;
  glm::vec3 GetScale() const;
  glm::quat GetRotation() const;

  glm::mat4 GetTransform() const override;

  Transform& Move(const glm::vec3 position);
  Transform& Scale(const glm::vec3 scale);
  Transform& Rotate(const glm::quat rotation);

  Transform& MoveBy(const glm::vec3 amount);

 private:
  const Transformable* parent_;
  glm::vec3 position_;
  glm::vec3 scale_;
  glm::quat rotation_;
};

#endif  // VULKAN_SRC_GAME_ACTOR_PROPERTY_TRANSFORM_H_
