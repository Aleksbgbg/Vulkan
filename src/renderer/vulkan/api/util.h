#ifndef VULKAN_SRC_RENDERER_VULKAN_API_UTIL_H_
#define VULKAN_SRC_RENDERER_VULKAN_API_UTIL_H_

#include <set>
#include <string>
#include <type_traits>
#include <vector>

#include "core/adapters/function_traits.h"
#include "core/types.h"

namespace vk {

template <typename TFunc>
auto LoadArray(const TFunc& func) {
  using TElement = std::remove_pointer_t<NthArgType<1U, TFunc>>;

  u32 count;
  func(&count, nullptr);

  std::vector<TElement> elements(count);
  func(&count, elements.data());

  return elements;
}

template <typename TElement, typename TFunc>
bool RequiredValuesAreAvailable(
    const std::vector<TElement>& availableElements,
    const std::vector<const char*>& requiredElements, const TFunc& transform) {
  std::set<std::string> requiredElementsSet(requiredElements.begin(),
                                            requiredElements.end());

  for (const TElement& element : availableElements) {
    requiredElementsSet.erase(transform(element));
  }

  return requiredElementsSet.empty();
}

}  // namespace vk

#endif  // VULKAN_SRC_RENDERER_VULKAN_API_UTIL_H_
