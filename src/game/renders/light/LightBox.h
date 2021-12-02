#ifndef VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTBOX_H_
#define VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTBOX_H_

#include "game/Actor.h"
#include "game/rendering/meshes/MeshRenderer.h"
#include "game/rendering/meshes/TexturedMesh.h"
#include "game/rendering/resources/ResourceLoader.h"

class LightBox : public Actor {
 public:
  LightBox() = default;
  LightBox(ResourceLoader& resourceLoader);

  const Mesh& GetMesh() const override;

  void UpdateModel(const UpdateContext& context) override;
  void Render(const MeshRenderer& renderer) const override;

 private:
  TexturedMesh mesh;
  glm::vec3 rotation;
};

#endif  // VULKAN_SRC_GAME_RENDERS_LIGHT_LIGHTBOX_H_
