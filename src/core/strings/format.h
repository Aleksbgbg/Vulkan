#ifndef SRC_CORE_STRINGS_FORMAT_H_
#define SRC_CORE_STRINGS_FORMAT_H_

#include <sstream>
#include <string>
#include <string_view>

namespace format {

template <typename T>
std::string ToString(const T value) {
  return std::to_string(value);
}

template <>
std::string ToString(const char* value);

template <>
std::string ToString(std::string_view value);

template <typename TArg0, typename... TArgs>
struct SerializeArgs {
  SerializeArgs(std::ostringstream& stream, const TArg0 arg0,
                const TArgs... args) {
    stream << ToString(arg0);
    SerializeArgs<TArgs...>(stream, args...);
  }
};

template <typename TArg0>
struct SerializeArgs<TArg0> {
  SerializeArgs(std::ostringstream& stream, const TArg0 arg0) {
    stream << ToString(arg0);
  }
};

}  // namespace format

template <typename... TArgs>
std::string Format(const TArgs... args) {
  std::ostringstream stream;
  format::SerializeArgs(stream, args...);
  return stream.str();
}

#endif  // SRC_CORE_STRINGS_FORMAT_H_
