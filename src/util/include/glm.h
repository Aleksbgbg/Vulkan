#ifndef VULKAN_SRC_UTIL_INCLUDE_GLM_H_
#define VULKAN_SRC_UTIL_INCLUDE_GLM_H_

#include <ostream>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

inline constexpr float HALF_TURN = glm::pi<float>();
inline constexpr float QUARTER_TURN = 0.5f * HALF_TURN;
inline constexpr float THREE_QUARTER_TURN = 1.5f * HALF_TURN;
inline constexpr float FULL_TURN = 2.0f * HALF_TURN;

inline constexpr glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
inline constexpr glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
inline constexpr glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);

inline constexpr glm::quat IDENTITY_ROTATION =
    glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

glm::quat Rotation(const float angle, glm::vec3 axis);

std::ostream& operator<<(std::ostream& stream, const glm::mat4& mat);

std::ostream& operator<<(std::ostream& stream, const glm::vec3 vec);

#endif  // VULKAN_SRC_UTIL_INCLUDE_GLM_H_
