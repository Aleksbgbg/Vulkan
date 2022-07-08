#ifndef SRC_CORE_LITERALS_H_
#define SRC_CORE_LITERALS_H_

#include <cstddef>

consteval std::size_t operator"" _sz(unsigned long long value) {
  return value;
}

#endif  // SRC_CORE_LITERALS_H_
