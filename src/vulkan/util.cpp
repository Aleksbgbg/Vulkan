#include "util.h"

#include <fstream>
#include <string>

std::vector<u8> ReadFile(const char* const filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error(std::string("Failed to open file '") + filename +
                             "'.");
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