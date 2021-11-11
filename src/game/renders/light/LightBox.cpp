#include "LightBox.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "game/rendering/vertices/PositionTextureVertex.h"
#include "util/filenames.h"

TexturedMesh LoadLightMesh(const ResourceLoader& resourceLoader) {
  DrawVertices<PositionTextureVertex> drawVertices =
      PositionTextureVertex::LoadVertices(SPHERE_MODEL_FILENAME);

  return TexturedMesh(
      resourceLoader.AllocateVertexBuffer(drawVertices.vertices),
      resourceLoader.AllocateIndexBuffer(drawVertices.indices),
      drawVertices.indices.size(),
      resourceLoader.LoadTexture(SUN_TEXTURE_FILENAME));
}

LightBox::LightBox(const ResourceLoader& resourceLoader)
    : mesh(LoadLightMesh(resourceLoader)), rotation(0.0f, 0.0f, -1.0f) {}

const Mesh& LightBox::GetMesh() const {
  return mesh;
}

void LightBox::UpdateModel(const UpdateContext& context) {
  rotation.x += context.deltaTime;
}

void LightBox::Render(const MeshRenderer& renderer) const {
  const glm::mat4 transform =
      glm::scale(glm::translate(glm::mat4(1.0f),
                                glm::vec3(0.0f, 0.0f, 1000.0f)),
                 glm::vec3(100.0f)) * glm::toMat4(glm::quat(rotation));

  renderer.RenderTransformed(mesh, transform);
}
