#ifndef VULKAN_SRC_VULKAN_ERROR_H
#define VULKAN_SRC_VULKAN_ERROR_H

#include "util/build_definition.h"

#if defined(DIAGNOSTICS)

#include <vulkan/vulkan.h>

#include <stdexcept>
#include <string>

class VulkanResultException : public std::runtime_error {
 public:
  VulkanResultException(const char* const file, const int line,
                        const char* const invocation, const VkResult result)
      : std::runtime_error(
            std::string() + "[" + file + ":" + std::to_string(line) + "] " +
            invocation + ": VkResult was not success. Was: " +
            std::to_string(result) + " but expected VK_SUCCESS (0).") {}
};

#define PROCEED_ON_VALID_RESULT(invocation)                                 \
  {                                                                         \
    const VkResult result = invocation;                                     \
    if (result != VK_SUCCESS) {                                             \
      throw VulkanResultException(__FILE__, __LINE__, #invocation, result); \
    }                                                                       \
  }

#else

#define PROCEED_ON_VALID_RESULT(invocation) invocation;

#endif

#endif  // VULKAN_SRC_VULKAN_ERROR_H
