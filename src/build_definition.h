#ifndef VULKAN_SRC_BUILD_DEFINITION_H
#define VULKAN_SRC_BUILD_DEFINITION_H

#ifdef NDEBUG
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

#endif  // VULKAN_SRC_BUILD_DEFINITION_H
