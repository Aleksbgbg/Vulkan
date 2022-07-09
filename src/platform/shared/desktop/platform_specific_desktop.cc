#include <iostream>

#include "core/diagnostics/log.h"
#include "core/files/file.h"

void BufferedLog(const std::string_view text) {
  std::cout << text << '\n';
}

void ImmediateLog(const std::string_view text) {
  std::cout << text << std::endl;
}

namespace file {

std::ifstream OpenAssetStream(const asset::Shader asset) {
  return OpenStream(ResolveAssetFilename(asset));
}
std::ifstream OpenAssetStream(const asset::Font asset) {
  return OpenStream(ResolveAssetFilename(asset));
}
std::ifstream OpenAssetStream(const asset::Model asset) {
  return OpenStream(ResolveAssetFilename(asset));
}
std::ifstream OpenAssetStream(const asset::Texture asset) {
  return OpenStream(ResolveAssetFilename(asset));
}
std::ifstream OpenAssetStream(const asset::Sound asset) {
  return OpenStream(ResolveAssetFilename(asset));
}

std::vector<u8> ReadAsset(const asset::Shader asset) {
  return ReadFile(ResolveAssetFilename(asset));
}
std::vector<u8> ReadAsset(const asset::Font asset) {
  return ReadFile(ResolveAssetFilename(asset));
}
std::vector<u8> ReadAsset(const asset::Model asset) {
  return ReadFile(ResolveAssetFilename(asset));
}
std::vector<u8> ReadAsset(const asset::Texture asset) {
  return ReadFile(ResolveAssetFilename(asset));
}
std::vector<u8> ReadAsset(const asset::Sound asset) {
  return ReadFile(ResolveAssetFilename(asset));
}

}  // namespace file
