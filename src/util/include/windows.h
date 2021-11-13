#ifndef VULKAN_SRC_INCLUDE_WINDOWS_H
#define VULKAN_SRC_INCLUDE_WINDOWS_H

#include <Windows.h>

// Bad Windows macros
#undef CreateSemaphore
#undef PostMessage
#undef min
#undef max
#undef CreateWindowA
#undef CreateWindowExA

#endif  // VULKAN_SRC_INCLUDE_WINDOWS_H
