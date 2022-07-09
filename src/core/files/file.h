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

std::ifstream OpenAssetStream(asset::Shader asset);
std::ifstream OpenAssetStream(asset::Font asset);
std::ifstream OpenAssetStream(asset::Model asset);
std::ifstream OpenAssetStream(asset::Texture asset);
std::ifstream OpenAssetStream(asset::Sound asset);

std::vector<u8> ReadAsset(asset::Shader asset);
std::vector<u8> ReadAsset(asset::Font asset);
std::vector<u8> ReadAsset(asset::Model asset);
std::vector<u8> ReadAsset(asset::Texture asset);
std::vector<u8> ReadAsset(asset::Sound asset);

std::ifstream OpenStream(std::string_view filename);

std::vector<u8> ReadFile(std::string_view filename);

void WriteFile(std::string_view filename, const std::string& data);
void WriteFile(std::string_view filename, const std::vector<u8>& data);

}  // namespace file

#endif  // SRC_CORE_FILES_FILE_H_
