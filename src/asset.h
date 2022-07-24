#ifndef SRC_ASSET_H_
#define SRC_ASSET_H_

namespace asset {

enum class Font {
  English,
  Korean,
  FontAwesome,
};

enum class Model {
  SkyboxSphere,
  Sphere,
  Transtellar,
  InterstellarRunner,
  Laser,
  ExhaustParticle,
};

enum class Shader {
  LightVert,
  LightFrag,
  ParticlesVert,
  ParticlesFrag,
  ParticlesComp,
  SkyboxVert,
  SkyboxFrag,
  SpaceshipVert,
  SpaceshipFrag,
  TextVert,
  TextFrag,
  UiVert,
  UiFrag,
};

enum class Sound {
  Background,
  Laser,
};

enum class Texture {
  InterstellarRunner,
  InterstellarRunnerEmissive,
  Transtellar,
  TranstellarEmissive,
  Nebula,
  Sun,
  Laser,
};

}  // namespace asset

#endif  // SRC_ASSET_H_
