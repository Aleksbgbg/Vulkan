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

std::string_view ResolveAssetFilename(const asset::Font asset) {
  static std::unordered_map<asset::Font, const char* const> assetFilenames{
      {asset::Font::English, "resources/fonts/SegoeUI.ttf"},
      {asset::Font::Korean, "resources/fonts/IBMPlexSansKR-Regular.ttf"},
      {asset::Font::FontAwesome, "resources/fonts/fontawesome-webfont.ttf"},
  };

  return assetFilenames.at(asset);
}

std::string_view ResolveAssetFilename(const asset::Model asset) {
  static std::unordered_map<asset::Model, const char* const> assetFilenames{
      {asset::Model::SkyboxSphere, "resources/models/SkyboxSphere.obj"},
      {asset::Model::Sphere, "resources/models/Sphere.obj"},
      {asset::Model::Transtellar, "resources/models/Transtellar.obj"},
      {asset::Model::InterstellarRunner,
       "resources/models/InterstellarRunner.obj"},
      {asset::Model::Laser, "resources/models/Laser.obj"},
      {asset::Model::ExhaustParticle, "resources/models/ExhaustParticle.obj"},
  };

  return assetFilenames.at(asset);
}

std::string_view ResolveAssetFilename(const asset::Shader asset) {
  static std::unordered_map<asset::Shader, const char* const> assetFilenames{
      {asset::Shader::LightVert, "resources/shaders/light.vert.spv"},
      {asset::Shader::LightFrag, "resources/shaders/light.frag.spv"},
      {asset::Shader::ParticlesVert, "resources/shaders/particles.vert.spv"},
      {asset::Shader::ParticlesFrag, "resources/shaders/particles.frag.spv"},
      {asset::Shader::ParticlesComp, "resources/shaders/particles.comp.spv"},
      {asset::Shader::SkyboxVert, "resources/shaders/skybox.vert.spv"},
      {asset::Shader::SkyboxFrag, "resources/shaders/skybox.frag.spv"},
      {asset::Shader::SpaceshipVert, "resources/shaders/spaceship.vert.spv"},
      {asset::Shader::SpaceshipFrag, "resources/shaders/spaceship.frag.spv"},
      {asset::Shader::TextVert, "resources/shaders/text.vert.spv"},
      {asset::Shader::TextFrag, "resources/shaders/text.frag.spv"},
      {asset::Shader::UiVert, "resources/shaders/ui.vert.spv"},
      {asset::Shader::UiFrag, "resources/shaders/ui.frag.spv"},
  };

  return assetFilenames.at(asset);
}

std::string_view ResolveAssetFilename(const asset::Sound asset) {
  static std::unordered_map<asset::Sound, const char* const> assetFilenames{
      {asset::Sound::Background, "resources/sounds/background.ogg"},
      {asset::Sound::Laser, "resources/sounds/laser.ogg"},
  };

  return assetFilenames.at(asset);
}

std::string_view ResolveAssetFilename(const asset::Texture asset) {
  static std::unordered_map<asset::Texture, const char* const> assetFilenames{
      {asset::Texture::InterstellarRunner,
       "resources/textures/InterstellarRunner.png"},
      {asset::Texture::InterstellarRunnerEmissive,
       "resources/textures/InterstellarRunner_Emissive.png"},
      {asset::Texture::Transtellar, "resources/textures/Transtellar.png"},
      {asset::Texture::TranstellarEmissive,
       "resources/textures/Transtellar_Emissive.png"},
      {asset::Texture::Nebula, "resources/textures/Nebula.png"},
      {asset::Texture::Sun, "resources/textures/Sun.png"},
      {asset::Texture::Laser, "resources/textures/Laser.png"},
  };

  return assetFilenames.at(asset);
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
