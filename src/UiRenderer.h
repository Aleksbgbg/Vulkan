#ifndef VULKAN_SRC_UIRENDERER_H
#define VULKAN_SRC_UIRENDERER_H

#include <array>

#include "vulkan/ImGuiInstance.h"
#include "GlmIncl.h"
#include "Iterable.h"
#include "Keyboard.h"

struct VulkanDebugInfo {
  char* const gpuName;
  float frametime;
};

struct ObjectsInSceneInfo {
  glm::vec2 cameraRotation;
  glm::vec3 cameraCenter;
  glm::vec3 cameraPosition;
  std::vector<glm::vec3*>& cubePositions;
};

class UiRenderer {
public:
  UiRenderer() = default;
  explicit UiRenderer(ImGuiInstance imGuiInstance);

  void ProcessEvent(const SDL_Event& event) const;

  void BeginFrame() const;
  void EndFrame() const;
  void Render(const CommandBuffer& commandBuffer) const;

  void ShowVulkanDebugInfo(const VulkanDebugInfo& info);
  void ShowObjectsInScene(const ObjectsInSceneInfo& info) const;
  void ShowKeyboardLayout(const Keyboard& keyboard) const;

private:
  struct KeyDescription{
    bool hasKey = true;
    const char* const key;
    SDL_Keycode code;
    const char* const description;
  };
  static void RenderKey(const Keyboard& keyboard, const KeyDescription& key);

  static float GetFrametimeHistoryValueFromRenderer(void* data, int index);
  float GetFrametimeHistoryValue(void* data, int index) const;

  ImGuiInstance imGuiInstance;

  u32 valuesFilled;
  u32 frametimeHistoryIndex;
  std::array<float, 10> frametimeHistory;
  float timeSum;

  std::array<std::vector<KeyDescription>, 2> sceneObjectControls;
};

#endif // VULKAN_SRC_UIRENDERER_H
