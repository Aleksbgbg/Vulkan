#include "glm.h"

#include "util/types.h"

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
