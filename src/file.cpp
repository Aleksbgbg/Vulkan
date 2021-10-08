#include "file.h"

#include <fstream>
#include <stdexcept>
#include <string>

std::runtime_error CannotOpen(const char* const filename) {
  return std::runtime_error(std::string("Failed to open file '") + filename +
                            "'.");
}

std::vector<u8> ReadFile(const char* const filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

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
void WriteFile(const char* const filename, const std::vector<u8>& data) {
  std::ofstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    throw CannotOpen(filename);
  }

  file.write(reinterpret_cast<const char*>(data.data()), data.size());
}
