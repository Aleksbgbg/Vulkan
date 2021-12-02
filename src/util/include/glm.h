#ifndef VULKAN_SRC_UTIL_INCLUDE_GLM_H_
#define VULKAN_SRC_UTIL_INCLUDE_GLM_H_

#include <ostream>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

inline constexpr float HalfTurn = glm::pi<float>();
inline constexpr float QuarterTurn = 0.5f * HalfTurn;
inline constexpr float ThreeQuarterTurn = 1.5f * HalfTurn;
inline constexpr float FullTurn = 2.0f * HalfTurn;

std::ostream& operator<<(std::ostream& stream, const glm::mat4& mat);

std::ostream& operator<<(std::ostream& stream, const glm::vec3 vec);

#endif  // VULKAN_SRC_UTIL_INCLUDE_GLM_H_
