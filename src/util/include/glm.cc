#include "glm.h"

#include "core/types.h"

glm::quat Rotation(const float angle, glm::vec3 axis) {
  axis = glm::normalize(axis);
  const float halfAngle = angle / 2.0f;
  const float sinHalfAngle = std::sin(halfAngle);
  return glm::quat(std::cos(halfAngle),
                   glm::vec3(axis.x * sinHalfAngle, axis.y * sinHalfAngle,
                             axis.z * sinHalfAngle));
}

std::ostream& operator<<(std::ostream& stream, const glm::mat4& mat) {
  stream << "glm::mat4{\n";

  for (u32 index = 0; index < 4; ++index) {
    auto vec = mat[index];
    stream << "\t" << vec.x << ", " << vec.y << ", " << vec.z << "\n";
  }

  return stream << "}\n";
}

std::ostream& operator<<(std::ostream& stream, const glm::vec3 vec) {
  return stream << "glm::vec3{ " << vec.x << ", " << vec.y << ", " << vec.z
                << "}";
}
