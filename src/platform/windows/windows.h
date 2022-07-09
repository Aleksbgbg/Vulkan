#ifndef SRC_UTIL_INCLUDE_WINDOWS_H_
#define SRC_UTIL_INCLUDE_WINDOWS_H_

#include <Windows.h>

// Bad Windows macros
#undef CreateSemaphore
#undef PostMessage
#undef min
#undef max
#undef CreateWindow
#undef CreateWindowA
#undef CreateWindowExA

#endif  // SRC_UTIL_INCLUDE_WINDOWS_H_
