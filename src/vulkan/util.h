#ifndef VULKAN_SRC_VULKAN_UTIL_H
#define VULKAN_SRC_VULKAN_UTIL_H

#include <functional>
#include <set>
#include <vector>

#include "types.h"

template <typename>
struct second_arg;

template <typename T2>
struct second_arg<std::function<void(u32*, T2*)>> {
  using type = T2;
};

template <typename TFunc>
auto LoadArray(const TFunc& func) {
  using TElement = typename second_arg<decltype(std::function{func})>::type;

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

#endif  // VULKAN_SRC_VULKAN_UTIL_H
