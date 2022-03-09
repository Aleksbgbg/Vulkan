#include "format.h"

#include <string>

namespace format {

template <>
std::string ToString<const char*>(const char* const value) {
  return std::string(value);
}

}  // namespace format
