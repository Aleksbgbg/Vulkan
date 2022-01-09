#ifndef VULKAN_SRC_RENDERER_VULKAN_API_ERROR_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_ERROR_H_

#include "util/build_definition.h"

#if defined(DIAGNOSTICS)

#include <vulkan/vulkan.h>

#include <stdexcept>
#include <string>

namespace vk {

class ResultException : public std::runtime_error {
 public:
  ResultException(const char* const file, const int line,
                  const char* const invocation, const VkResult result)
      : std::runtime_error(
            std::string() + "[" + file + ":" + std::to_string(line) + "] " +
            invocation + ": VkResult was not success. Was: " +
            std::to_string(result) + " but expected VK_SUCCESS (0).") {}
};

}  // namespace vk

#define PROCEED_ON_VALID_RESULT(invocation)                               \
  {                                                                       \
    const VkResult result = invocation;                                   \
    if (result != VK_SUCCESS) {                                           \
      throw vk::ResultException(__FILE__, __LINE__, #invocation, result); \
    }                                                                     \
  }

#else

#define PROCEED_ON_VALID_RESULT(invocation) invocation;

#endif

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_ERROR_H_
