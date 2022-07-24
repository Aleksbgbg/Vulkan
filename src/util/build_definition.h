#ifndef SRC_UTIL_BUILD_DEFINITION_H_
#define SRC_UTIL_BUILD_DEFINITION_H_

#if defined(NDEBUG) || defined(__ANDROID__)
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

#endif  // SRC_UTIL_BUILD_DEFINITION_H_
