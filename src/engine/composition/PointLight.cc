#include "PointLight.h"

PointLight::PointLight()
    : ambient_(),
      diffuse_(),
      specular_(),
      attenuationConstant_(1.0f),
      attenuationLinear_(0.0f),
      attenuationQuadratic_(0.0f) {}

LightSource PointLight::GetLightSource() const {
  return {.type = LightSourceType::PointLight,
          .info = {.pointLight = {
                       .ambient = ambient_,
                       .diffuse = diffuse_,
                       .specular = specular_,
                       .attenuation = {.constant = attenuationConstant_,
                                       .linear = attenuationLinear_,
                                       .quadratic = attenuationQuadratic_}}}};
}

PointLight& PointLight::Ambient(const glm::vec3 value) {
  ambient_ = value;
  return *this;
}

PointLight& PointLight::Diffuse(const glm::vec3 value) {
  diffuse_ = value;
  return *this;
}

PointLight& PointLight::Specular(const glm::vec3 value) {
  specular_ = value;
  return *this;
}

PointLight& PointLight::Color(const glm::vec3 value) {
  return Ambient(value).Diffuse(value).Specular(value);
}

PointLight& PointLight::AmbientFactor(const float factor) {
  ambient_ *= factor;
  return *this;
}

PointLight& PointLight::AttenuationConstant(const float value) {
  attenuationConstant_ = value;
  return *this;
}

PointLight& PointLight::AttenuationLinear(const float value) {
  attenuationLinear_ = value;
  return *this;
}

PointLight& PointLight::AttenuationQuadratic(const float value) {
  attenuationQuadratic_ = value;
  return *this;
}
