#ifndef VULKAN_SRC_UTIL_BUILD_DEFINITION_H_
#define VULKAN_SRC_UTIL_BUILD_DEFINITION_H_

#if defined(NDEBUG)
#define RELEASE
#else
#define DEBUG
#endif

#if defined(DEBUG)
#define VALIDATION
#define DIAGNOSTICS
#define TOOLS
#define PROFILING
#endif

#if defined(RELEASE)
// #define VALIDATION
#define DIAGNOSTICS
// #define TOOLS
#define PROFILING
#endif

#endif  // VULKAN_SRC_UTIL_BUILD_DEFINITION_H_
