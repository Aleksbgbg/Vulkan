#ifndef VULKAN_SRC_UIRENDERER_H
#define VULKAN_SRC_UIRENDERER_H

#include <array>

#include "Iterable.h"
#include "Keyboard.h"
#include "Rect.h"
#include "include_glm.h"
#include "vulkan/ImGuiInstance.h"

struct VulkanDebugInfo {
  char* const gpuName;
  float frametime;
};

struct ObjectsInSceneInfo {
  glm::vec2 cameraRotation;
  glm::vec3 cameraCenter;
  glm::vec3 cameraPosition;
  std::vector<glm::vec3*>& cubePositions;
  u32* selectedObjectIndex;
};

class UiRenderer {
 public:
  UiRenderer() = default;
  explicit UiRenderer(Rectf windowSize, ImGuiInstance imGuiInstance);

  void ProcessEvent(const SDL_Event& event) const;

  void BeginFrame() const;
  void EndFrame() const;
  void Render(const CommandBuffer& commandBuffer) const;

  void ShowVulkanDebugInfo(const VulkanDebugInfo& info);
  void ShowObjectsInScene(const ObjectsInSceneInfo& info);
  void ShowKeyboardLayout(Keyboard& keyboard);

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

  bool autoSelectLastCube;
  u32 selectedCube;
  u32 lastCubeCount;

  std::array<std::vector<KeyDescription>, 2> sceneObjectControls;
  std::array<KeyDescription, 4> cameraKeys;
  KeyDescription spawnCubeKey;
};

#endif  // VULKAN_SRC_UIRENDERER_H
