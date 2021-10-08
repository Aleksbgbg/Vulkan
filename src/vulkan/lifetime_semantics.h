#ifndef VULKAN_SRC_VULKAN_LIFETIME_SEMANTICS_H
#define VULKAN_SRC_VULKAN_LIFETIME_SEMANTICS_H

#include <utility>

#define VULKAN_OBJECT_MOVABLE(className, mainPointer)                      \
  className(className&& other) noexcept : mainPointer(other.mainPointer) { \
    other.mainPointer = nullptr;                                           \
  }                                                                        \
                                                                           \
  className& operator=(className&& other) noexcept {                       \
    std::swap(mainPointer, other.mainPointer);                             \
    return *this;                                                          \
  }

#define VULKAN_OBJECT_MOVABLE_ROOT(className, rootPointer, mainPointer)  \
  className(className&& other) noexcept                                  \
      : mainPointer(other.mainPointer), rootPointer(other.rootPointer) { \
    other.mainPointer = nullptr;                                         \
  }                                                                      \
                                                                         \
  className& operator=(className&& other) noexcept {                     \
    rootPointer = other.rootPointer;                                     \
    std::swap(mainPointer, other.mainPointer);                           \
    return *this;                                                        \
  }

#endif  // VULKAN_SRC_VULKAN_LIFETIME_SEMANTICS_H
