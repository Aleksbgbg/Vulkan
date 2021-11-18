#include "Skybox.h"

#include "game/rendering/vertices/PositionTextureVertex.h"
#include "util/filenames.h"

TexturedMesh LoadSkyboxMesh(ResourceLoader& resourceLoader) {
  DrawVertices<PositionTextureVertex> drawVertices =
      PositionTextureVertex::LoadVertices(SPHERE_MODEL_FILENAME);

  const glm::mat4 enlarge = glm::scale(glm::mat4(1.0f), glm::vec3(2000.0f));

  for (PositionTextureVertex& vertex : drawVertices.vertices) {
    vertex.position = enlarge * glm::vec4(vertex.position, 1.0f);
  }

  return TexturedMesh(
      resourceLoader.AllocateVertexBuffer(drawVertices.vertices),
      resourceLoader.AllocateIndexBuffer(drawVertices.indices),
      drawVertices.indices.size(),
      resourceLoader.LoadTexture(SKYBOX_TEXTURE_FILENAME));
}

Skybox::Skybox(ResourceLoader& resourceLoader)
    : mesh(LoadSkyboxMesh(resourceLoader)) {}

const Mesh& Skybox::GetMesh() const {
  return mesh;
}

void Skybox::UpdateModel(const UpdateContext& context) {}

void Skybox::Render(const MeshRenderer& renderer) const {
  renderer.Render(mesh);
}
