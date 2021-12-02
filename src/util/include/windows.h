#ifndef VULKAN_SRC_UTIL_INCLUDE_WINDOWS_H_
#define VULKAN_SRC_UTIL_INCLUDE_WINDOWS_H_

#include <Windows.h>

// Bad Windows macros
#undef CreateSemaphore
#undef PostMessage
#undef min
#undef max
#undef CreateWindowA
#undef CreateWindowExA

#endif  // VULKAN_SRC_UTIL_INCLUDE_WINDOWS_H_
