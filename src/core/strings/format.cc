#include "format.h"

#include <string>

namespace format {

template <>
std::string ToString(const char* const value) {
  return std::string(value);
}

template <>
std::string ToString(std::string_view value) {
  return std::string(value);
}

}  // namespace format
