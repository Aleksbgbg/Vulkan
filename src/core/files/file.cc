#include "file.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace {

std::runtime_error CannotOpen(const std::string_view filename) {
  return std::runtime_error(std::string("Failed to open file '") +
                            filename.data() + "'.");
}

}  // namespace

namespace file {

std::string_view ResolveAssetFilename(const asset::Shader asset) {
  static std::unordered_map<asset::Shader, const char* const> assetFilenames{
      {asset::Shader::LightVert, "shaders/light.vert.spv"},
      {asset::Shader::LightFrag, "shaders/light.frag.spv"},
      {asset::Shader::ParticlesVert, "shaders/particles.vert.spv"},
      {asset::Shader::ParticlesFrag, "shaders/particles.frag.spv"},
      {asset::Shader::ParticlesComp, "shaders/particles.comp.spv"},
      {asset::Shader::SkyboxVert, "shaders/skybox.vert.spv"},
      {asset::Shader::SkyboxFrag, "shaders/skybox.frag.spv"},
      {asset::Shader::SpaceshipVert, "shaders/spaceship.vert.spv"},
      {asset::Shader::SpaceshipFrag, "shaders/spaceship.frag.spv"},
      {asset::Shader::TextVert, "shaders/text.vert.spv"},
      {asset::Shader::TextFrag, "shaders/text.frag.spv"},
      {asset::Shader::UiVert, "shaders/ui.vert.spv"},
      {asset::Shader::UiFrag, "shaders/ui.frag.spv"},
  };

  return assetFilenames.at(asset);
}

std::string_view file::ResolveAssetFilename(const asset::Font asset) {
  static std::unordered_map<asset::Font, const char* const> assetFilenames{
      {asset::Font::English, "resources/SegoeUI.ttf"},
      {asset::Font::Korean, "resources/IBMPlexSansKR-Regular.ttf"},
      {asset::Font::FontAwesome, "resources/fontawesome-webfont.ttf"},
  };

  return assetFilenames.at(asset);
}

std::string_view file::ResolveAssetFilename(const asset::Model asset) {
  static std::unordered_map<asset::Model, const char* const> assetFilenames{
      {asset::Model::SkyboxSphere, "resources/SkyboxSphere.obj"},
      {asset::Model::Sphere, "resources/Sphere.obj"},
      {asset::Model::Transtellar, "resources/Transtellar.obj"},
      {asset::Model::InterstellarRunner, "resources/InterstellarRunner.obj"},
      {asset::Model::Laser, "resources/Laser.obj"},
      {asset::Model::ExhaustParticle, "resources/ExhaustParticle.obj"},
  };

  return assetFilenames.at(asset);
}

std::string_view file::ResolveAssetFilename(const asset::Texture asset) {
  static std::unordered_map<asset::Texture, const char* const> assetFilenames{
      {asset::Texture::InterstellarRunner, "resources/InterstellarRunner.png"},
      {asset::Texture::InterstellarRunnerEmissive,
       "resources/InterstellarRunner_Emissive.png"},
      {asset::Texture::Transtellar, "resources/Transtellar.png"},
      {asset::Texture::TranstellarEmissive,
       "resources/Transtellar_Emissive.png"},
      {asset::Texture::Nebula, "resources/Nebula.png"},
      {asset::Texture::Sun, "resources/Sun.png"},
      {asset::Texture::Laser, "resources/Laser.png"},
  };

  return assetFilenames.at(asset);
}

std::string_view file::ResolveAssetFilename(const asset::Sound asset) {
  static std::unordered_map<asset::Sound, const char* const> assetFilenames{
      {asset::Sound::Background, "resources/background.ogg"},
      {asset::Sound::Laser, "resources/laser.ogg"},
  };

  return assetFilenames.at(asset);
}

std::ifstream OpenStream(const std::string_view filename) {
  std::ifstream file(filename.data());

  if (!file.is_open()) {
    throw CannotOpen(filename);
  }

  return file;
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
