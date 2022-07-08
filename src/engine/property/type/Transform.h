#ifndef SRC_ENGINE_PROPERTY_TYPE_TRANSFORM_H_
#define SRC_ENGINE_PROPERTY_TYPE_TRANSFORM_H_

#include "engine/property/Property.h"
#include "engine/property/PropertyKey.h"
#include "engine/property/traits/Transformable.h"
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

#endif  // SRC_ENGINE_PROPERTY_TYPE_TRANSFORM_H_
