#ifndef VULKAN_SRC_GAME_SCENE_H
#define VULKAN_SRC_GAME_SCENE_H

#include <vector>

#include "Camera.h"
#include "DynamicUniformBuffer.h"
#include "VulkanContext.h"
#include "game/SceneDescriptor.h"
#include "game/model/UpdateContext.h"
#include "game/renders/SceneRenderer.h"
#include "general/windowing/Window.h"
#include "vulkan/CommandBuffer.h"

class Scene {
 public:
  Scene() = default;
  Scene(const VulkanContext& vulkanContext,
        const ResourceLoader& resourceLoader, const Window& window,
        const u32& imageIndex,
        const DynamicUniformBufferInitializer& uniformBufferInitializer);

  Scene(const Scene&) = delete;
  Scene(Scene&&) = delete;

  Scene& operator=(const Scene&) = delete;
  Scene& operator=(Scene&&) = delete;

  const Camera& GetCamera() const;

  void UpdateAspect(const float aspect);

  void UpdateModel(const UpdateContext& context);
  void Render(const CommandBuffer& commandBuffer) const;

 private:
  std::vector<SceneRenderer> renderers;

  const Window* window;
  Camera camera;

  DescriptorPool descriptorPool;
  SceneDescriptor sceneDescriptor;
};

#endif  // VULKAN_SRC_GAME_SCENE_H
