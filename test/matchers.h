#ifndef VULKAN_TEST_MATCHERS_H_
#define VULKAN_TEST_MATCHERS_H_

#include <gmock/gmock.h>

namespace matchers {

MATCHER_P2(EqualsMemoryRegion, region, size, "") {
  return std::memcmp(arg, region, size) == 0;
}

}  // namespace matchers

#endif  // VULKAN_TEST_MATCHERS_H_
