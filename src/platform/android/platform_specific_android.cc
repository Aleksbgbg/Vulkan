#include <SDL_rwops.h>
#include <SDL_system.h>
#include <android/log.h>

#include <streambuf>

#include "core/diagnostics/log.h"
#include "core/files/file.h"
#include "core/strings/format.h"
#include "util/project.h"

namespace {

static constexpr const char* READ_BINARY = "rb";

std::vector<u8> ReadAndroidAsset(const std::string_view filename) {
  SDL_RWops* fileHandle = SDL_RWFromFile(filename.data(), READ_BINARY);

  if (fileHandle == nullptr) {
    throw std::runtime_error(
        Format("Could not read resource ", filename.data()));
  }

  const i64 resourceSize = SDL_RWsize(fileHandle);

  ImmediateLog(Format("Reading resource ", filename.data(),
                      " which is of size ", resourceSize));

  std::vector<u8> response(resourceSize);

  i64 bytesReadTotal = 0;
  i64 bytesRead = 1;
  u8* buffer = response.data();

  while ((bytesReadTotal < resourceSize) && (bytesRead != 0)) {
    bytesRead = SDL_RWread(fileHandle, buffer, sizeof(u8),
                           resourceSize - bytesReadTotal);
    bytesReadTotal += bytesRead;
    buffer += bytesRead;
  }
  SDL_RWclose(fileHandle);

  if (bytesReadTotal != resourceSize) {
    throw std::runtime_error(Format(
        "Could not read all of the bytes of resource ", filename.data()));
  }

  return response;
}

}  // namespace

void BufferedLog(const std::string_view text) {
  ImmediateLog(text);
}

void ImmediateLog(const std::string_view text) {
  __android_log_print(ANDROID_LOG_INFO, PROJECT_TITLE, "%s\n", text.data());
}

namespace file {

std::string RelativeApplicationPath(const std::string_view path) {
  return Format(SDL_AndroidGetInternalStoragePath(), path);
}

std::vector<u8> ReadAsset(const asset::Font asset) {
  return ReadAndroidAsset(ResolveAssetFilename(asset));
}
std::vector<u8> ReadAsset(const asset::Model asset) {
  return ReadAndroidAsset(ResolveAssetFilename(asset));
}
std::vector<u8> ReadAsset(const asset::Shader asset) {
  return ReadAndroidAsset(ResolveAssetFilename(asset));
}
std::vector<u8> ReadAsset(const asset::Sound asset) {
  return ReadAndroidAsset(ResolveAssetFilename(asset));
}
std::vector<u8> ReadAsset(const asset::Texture asset) {
  return ReadAndroidAsset(ResolveAssetFilename(asset));
}

}  // namespace file
