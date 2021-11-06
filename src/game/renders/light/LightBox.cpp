#include "LightBox.h"

#include "game/rendering/vertices/PositionTextureVertex.h"
#include "util/filenames.h"

TexturedMesh LoadLightMesh(const ResourceLoader& resourceLoader) {
  DrawVertices<PositionTextureVertex> drawVertices =
      PositionTextureVertex::LoadVertices(SPHERE_MODEL_FILENAME);

  const glm::mat4 transform = glm::scale(
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1000.0f)),
      glm::vec3(100.0f));

  for (PositionTextureVertex& vertex : drawVertices.vertices) {
    vertex.position = transform * glm::vec4(vertex.position, 1.0f);
  }

  return TexturedMesh(
      resourceLoader.AllocateVertexBuffer(drawVertices.vertices),
      resourceLoader.AllocateIndexBuffer(drawVertices.indices),
      drawVertices.indices.size(),
      resourceLoader.LoadTexture(SUN_TEXTURE_FILENAME));
}

LightBox::LightBox(const ResourceLoader& resourceLoader)
    : lightMesh(LoadLightMesh(resourceLoader)) {}

void LightBox::UpdateModel(const UpdateContext& context) {}

void LightBox::WriteTexture(TextureRegistry& textureRegistry) const {
  lightMesh.WriteTexture(textureRegistry);
}

void LightBox::Render(const MeshRenderer& renderer) const {
  renderer.Render(lightMesh, glm::mat4(1.0f));
}
