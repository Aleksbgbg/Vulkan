#include "LightBox.h"

#include "LightVertex.h"

LightMesh LoadLightMesh(const ResourceLoader& resourceLoader) {
  constexpr const float offsetX = 0.0f;
  constexpr const float offsetY = 0.0f;
  constexpr const float offsetZ = 1000.0f;

  constexpr const float scale = 100.0f;

  const std::vector<LightVertex> vertices = {
      /* 0 */ {{-scale + offsetX, -scale + offsetY, -scale + offsetZ}},
      /* 1 */ {{scale + offsetX, -scale + offsetY, -scale + offsetZ}},
      /* 2 */ {{-scale + offsetX, scale + offsetY, -scale + offsetZ}},
      /* 3 */ {{scale + offsetX, scale + offsetY, -scale + offsetZ}},
      /* 4 */ {{-scale + offsetX, -scale + offsetY, scale + offsetZ}},
      /* 5 */ {{scale + offsetX, -scale + offsetY, scale + offsetZ}},
      /* 6 */ {{-scale + offsetX, scale + offsetY, scale + offsetZ}},
      /* 7 */ {{scale + offsetX, scale + offsetY, scale + offsetZ}},
  };
  const std::vector<u16> indices = {
      0, 2, 1, 1, 2, 3,  // bottom face
      0, 5, 4, 0, 1, 5,  // back face
      1, 3, 7, 1, 7, 5,  // left face
      0, 6, 2, 0, 4, 6,  // right face
      2, 6, 7, 2, 7, 3,  // front face
      4, 5, 6, 5, 7, 6,  // top face
  };

  return LightMesh(resourceLoader.AllocateDeviceBuffer(
                       vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT),
                   resourceLoader.AllocateDeviceBuffer(
                       indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT),
                   indices.size());
}

LightBox::LightBox(const ResourceLoader& resourceLoader)
    : lightMesh(LoadLightMesh(resourceLoader)) {}

void LightBox::UpdateModel(const UpdateContext& context) {}

void LightBox::WriteTexture(TextureRegistry& textureRegistry) const {}

void LightBox::Render(const MeshRenderer& renderer) const {
  renderer.Render(lightMesh, glm::mat4(1.0f));
}
