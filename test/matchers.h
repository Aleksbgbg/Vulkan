#ifndef TEST_MATCHERS_H_
#define TEST_MATCHERS_H_

#include <gmock/gmock.h>

namespace matchers {

MATCHER_P2(EqualsMemoryRegion, region, size, "") {
  return std::memcmp(arg, region, size) == 0;
}

}  // namespace matchers

#endif  // TEST_MATCHERS_H_
