#ifndef VULKAN_SRC_INCLUDE_GLM_H
#define VULKAN_SRC_INCLUDE_GLM_H

#include <ostream>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::ostream& operator<<(std::ostream& stream, const glm::mat4& mat);

std::ostream& operator<<(std::ostream& stream, const glm::vec3 vec);

#endif  // VULKAN_SRC_INCLUDE_GLM_H
