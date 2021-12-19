#ifndef VULKAN_SRC_GAME_COMPOSITION_POINTLIGHT_H_
#define VULKAN_SRC_GAME_COMPOSITION_POINTLIGHT_H_

#include "LightSourceComposition.h"
#include "game/LightSource.h"
#include "util/include/glm.h"

inline constexpr glm::vec3 BLACK = glm::vec3(0.0f);
inline constexpr glm::vec3 WHITE = glm::vec3(1.0f);

class PointLight : public LightSourceComposition {
 public:
  PointLight();

  LightSource GetLightSource() const override;

  PointLight& Ambient(glm::vec3 value);
  PointLight& Diffuse(glm::vec3 value);
  PointLight& Specular(glm::vec3 value);

  PointLight& Color(glm::vec3 value);
  PointLight& AmbientFactor(float factor);

  PointLight& AttenuationConstant(float value);
  PointLight& AttenuationLinear(float value);
  PointLight& AttenuationQuadratic(float value);

 private:
  glm::vec3 ambient_;
  glm::vec3 diffuse_;
  glm::vec3 specular_;

  float attenuationConstant_;
  float attenuationLinear_;
  float attenuationQuadratic_;
};

#endif  // VULKAN_SRC_GAME_COMPOSITION_POINTLIGHT_H_
