#ifndef SRC_CORE_FILES_FILE_H_
#define SRC_CORE_FILES_FILE_H_

#include <fstream>
#include <string_view>
#include <vector>

#include "asset.h"
#include "core/types.h"

namespace file {

std::string_view ResolveAssetFilename(asset::Shader asset);
std::string_view ResolveAssetFilename(asset::Font asset);
std::string_view ResolveAssetFilename(asset::Model asset);
std::string_view ResolveAssetFilename(asset::Texture asset);
std::string_view ResolveAssetFilename(asset::Sound asset);

std::ifstream OpenStream(std::string_view filename);
template <typename Asset>
std::ifstream OpenAssetStream(const Asset asset) {
  return OpenStream(ResolveAssetFilename(asset));
}

std::vector<u8> ReadFile(std::string_view filename);
template <typename Asset>
std::vector<u8> ReadAsset(const Asset asset) {
  return ReadFile(ResolveAssetFilename(asset));
}

void WriteFile(std::string_view filename, const std::string& data);
void WriteFile(std::string_view filename, const std::vector<u8>& data);

}  // namespace file

#endif  // SRC_CORE_FILES_FILE_H_
