#ifndef VULKAN_SRC_UIRENDERER_H
#define VULKAN_SRC_UIRENDERER_H

#include <array>

#include "general/geometry/Rect.h"
#include "general/windowing/Window.h"
#include "general/windowing/input/Keyboard.h"
#include "util/include/glm.h"
#include "vulkan/ImGuiInstance.h"

struct VulkanDebugInfo {
  char* const gpuName;
  float frametime;
};

struct ObjectsInSceneInfo {
  glm::vec2 cameraRotation;
  glm::vec3 cameraLookAt;
  glm::vec3 cameraPosition;

  glm::vec3* modelPosition;
};

class UiRenderer {
 public:
  UiRenderer() = default;
  explicit UiRenderer(Rectf windowSize, ImGuiInstance imGuiInstance);

  void BeginFrame() const;
  void EndFrame() const;
  void Render(const CommandBuffer& commandBuffer) const;

  void ShowVulkanDebugInfo(const VulkanDebugInfo& info);
  void ShowObjectsInScene(const ObjectsInSceneInfo& info);
  void ShowKeyboardLayout(Window& window);

 private:
  struct KeyDescription {
    KeyDescription()
        : hasKey(false),
          key(nullptr),
          code(0),
          description(nullptr),
          pressed(false) {}
    KeyDescription(const char* const key, SDL_KeyCode code,
                   const char* description)
        : hasKey(true),
          key(key),
          code(code),
          description(description),
          pressed(false) {}

    bool hasKey;
    const char* key;
    SDL_Keycode code;
    const char* description;
    bool pressed;
  };
  static void RenderKey(Keyboard& keyboard, KeyDescription& key);

  static float GetFrametimeHistoryValueFromRenderer(void* data, int index);
  float GetFrametimeHistoryValue(void* data, int index) const;

  Rectf windowSize;
  ImGuiInstance imGuiInstance;

  u32 valuesFilled;
  u32 frametimeHistoryIndex;
  std::array<float, 10> frametimeHistory;
  float timeSum;

  std::array<std::vector<KeyDescription>, 2> sceneObjectControls;
  KeyDescription lookBackKey;
};

#endif  // VULKAN_SRC_UIRENDERER_H
