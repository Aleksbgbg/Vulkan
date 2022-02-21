#include "file.h"

#include <fstream>
#include <stdexcept>
#include <string>

namespace file {

std::runtime_error CannotOpen(const std::string_view filename) {
  return std::runtime_error(std::string("Failed to open file '") +
                            filename.data() + "'.");
}

std::vector<u8> ReadFile(const std::string_view filename) {
  std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw CannotOpen(filename);
  }

  const u32 fileSize = file.tellg();
  std::vector<u8> buffer(fileSize);
  file.seekg(0);
  file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

  if (buffer.size() != fileSize) {
    throw std::runtime_error("Buffer size " + std::to_string(buffer.size()) +
                             "is not file size " + std::to_string(fileSize));
  }

  return buffer;
}

void WriteFile(const std::string_view filename, const std::string& data) {
  std::ofstream file(filename.data());

  if (!file.is_open()) {
    throw CannotOpen(filename);
  }

  file.write(data.c_str(), data.size());
}

void WriteFile(const std::string_view filename, const std::vector<u8>& data) {
  std::ofstream file(filename.data(), std::ios::binary);

  if (!file.is_open()) {
    throw CannotOpen(filename);
  }

  file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

}  // namespace file
