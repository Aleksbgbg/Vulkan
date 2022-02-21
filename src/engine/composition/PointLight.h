#ifndef VULKAN_SRC_ENGINE_COMPOSITION_POINTLIGHT_H_
#define VULKAN_SRC_ENGINE_COMPOSITION_POINTLIGHT_H_

#include "LightSource.h"
#include "LightSourceComposition.h"
#include "util/include/glm.h"

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

#endif  // VULKAN_SRC_ENGINE_COMPOSITION_POINTLIGHT_H_
