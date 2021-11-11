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

class UiRenderer {
 public:
  UiRenderer() = default;
  explicit UiRenderer(Rectf windowSize, ImGuiInstance imGuiInstance);

  void BeginFrame() const;
  void EndFrame() const;
  void Render(const CommandBuffer& commandBuffer) const;

  void ShowVulkanDebugInfo(const VulkanDebugInfo& info);

 private:
  static float GetFrametimeHistoryValueFromRenderer(void* data, int index);
  float GetFrametimeHistoryValue(void* data, int index) const;

  Rectf windowSize;
  ImGuiInstance imGuiInstance;

  u32 valuesFilled;
  u32 frametimeHistoryIndex;
  std::array<float, 10> frametimeHistory;
  float timeSum;
};

#endif  // VULKAN_SRC_UIRENDERER_H
