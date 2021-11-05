#include "Skybox.h"

#include "game/renders/sky/SkyboxVertex.h"
#include "util/filenames.h"

SkyboxMesh LoadSkyboxMesh(const ResourceLoader& resourceLoader) {
  constexpr const float width = 4096.0f;
  constexpr const float height = 2048.0f;
  constexpr const float depth = 1000.0f;

  constexpr const float x = width / 2.0f;
  constexpr const float y = height / 2.0f;

  const std::vector<SkyboxVertex> vertices{
      /* 0  */ {{-x, -y, depth}, {1.0f, 0.25f}},
      /* 1  */ {{-x, y, depth}, {1.0f, 0.5f}},
      /* 2  */ {{x, -y, depth}, {0.0f, 0.25f}},
      /* 3  */ {{x, y, depth}, {0.0f, 0.5f}},

      /* 4  */ {{-x, -y, -depth}, {1.0f, 1.0f}},
      /* 5  */ {{-x, y, -depth}, {1.0f, 0.75f}},
      /* 6  */ {{x, -y, -depth}, {0.0f, 1.0f}},
      /* 7  */ {{x, y, -depth}, {0.0f, 0.75f}},

      /* 8  */ {{x, -y, depth}, {0.0f, 0.0f}},
      /* 9  */ {{x, y, depth}, {0.0f, 1.0f}},
      /* 10 */ {{x, -y, -depth}, {1.0f, 0.0f}},
      /* 11 */ {{x, y, -depth}, {1.0f, 1.0f}},

      /* 12 */ {{-x, -y, depth}, {0.0f, 0.0f}},
      /* 13 */ {{-x, y, depth}, {0.0f, 1.0f}},
      /* 14 */ {{-x, -y, -depth}, {1.0f, 0.0f}},
      /* 15 */ {{-x, y, -depth}, {1.0f, 1.0f}},

      /* 16 */ {{x, y, depth}, {0.0f, 0.5f}},
      /* 17 */ {{-x, y, depth}, {1.0f, 0.5f}},
      /* 18 */ {{x, y, -depth}, {0.0f, 0.75f}},
      /* 19 */ {{-x, y, -depth}, {1.0f, 0.75f}},

      /* 20 */ {{x, -y, depth}, {0.0f, 0.25f}},
      /* 21 */ {{-x, -y, depth}, {1.0f, 0.25f}},
      /* 22 */ {{x, -y, -depth}, {0.0f, 1.0f}},
      /* 23 */ {{-x, -y, -depth}, {1.0f, 1.0f}},
  };
  const std::vector<u16> indices{
      0,  1,  2,  1,  3,  2,   // Front
      4,  6,  5,  5,  6,  7,   // Back
      8,  9,  10, 9,  11, 10,  // Left
      12, 14, 13, 13, 14, 15,  // Right
      16, 17, 18, 17, 19, 18,  // Top
      20, 22, 21, 21, 22, 23,  // Bottom
  };

  return SkyboxMesh(resourceLoader.AllocateDeviceBuffer(
                        vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT),
                    resourceLoader.AllocateDeviceBuffer(
                        indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT),
                    indices.size(),
                    resourceLoader.LoadTexture(SKYBOX_TEXTURE_FILENAME));
}

Skybox::Skybox(const ResourceLoader& resourceLoader)
    : mesh(LoadSkyboxMesh(resourceLoader)) {}

void Skybox::WriteTexture(TextureRegistry& textureRegistry) const {
  mesh.WriteTexture(textureRegistry);
}

void Skybox::UpdateModel(const UpdateContext& context) {}

void Skybox::Render(const MeshRenderer& renderer) const {
  renderer.Render(mesh, glm::mat4(1.0f));
}
