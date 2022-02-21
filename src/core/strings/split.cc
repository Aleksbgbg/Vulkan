#include "split.h"

std::vector<std::string> Split(const std::string_view string,
                               const std::string_view delimiter) {
  std::vector<std::string> parts;

  std::size_t lastMatch = 0;

  while (true) {
    const std::size_t newMatch = string.find(delimiter, lastMatch);

    if (newMatch == std::string::npos) {
      parts.push_back(
          std::string(string.substr(lastMatch, string.size() - lastMatch)));
      break;
    } else {
      parts.push_back(
          std::string(string.substr(lastMatch, newMatch - lastMatch)));
    }

    lastMatch = newMatch + delimiter.size();
  }

  return parts;
}
