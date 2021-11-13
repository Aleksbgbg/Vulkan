#ifndef VULKAN_SRC_BUILD_DEFINITION_H
#define VULKAN_SRC_BUILD_DEFINITION_H

// Compilation depending on debug / release

#if defined(NDEBUG)
#define RELEASE
#else
#define DEBUG
#endif

#if defined(DEBUG)
#define VALIDATION
#define DIAGNOSTICS
#define TOOLS
#endif

#if defined(RELEASE)
// #define VALIDATION
#define DIAGNOSTICS
// #define TOOLS
#endif

// Compilation depending on platform

#if defined(__clang__)
#define LINUX
#elif defined(__GNUC__) || defined(__GNUG__)
#define LINUX
#elif defined(_MSC_VER)
#define WINDOWS
#endif

#endif  // VULKAN_SRC_BUILD_DEFINITION_H
