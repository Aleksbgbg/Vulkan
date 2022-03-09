#ifndef VULKAN_SRC_CORE_LITERALS_H_
#define VULKAN_SRC_CORE_LITERALS_H_

#include <cstddef>

consteval std::size_t operator"" _sz(unsigned long long value) {
  return value;
}

#endif  // VULKAN_SRC_CORE_LITERALS_H_
