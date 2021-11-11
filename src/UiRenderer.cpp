#include "UiRenderer.h"

#include <imgui.h>

#include <string>

#include "util/build_definition.h"

static constexpr float Margin = 1.0f;

float Average(const float* const values, const u32 size) {
  float sum = 0.0f;

  for (u32 index = 0; index < size; ++index) {
    sum += values[index];
  }

  return sum / size;
}

static constexpr float ButtonSize = 50.0f;

UiRenderer::UiRenderer(Rectf windowSize, ImGuiInstance imGuiInstance)
    : windowSize(windowSize),
      imGuiInstance(std::move(imGuiInstance)),
      valuesFilled(0),
      frametimeHistoryIndex(0),
      frametimeHistory(),
      timeSum(0.0f) {}

void UiRenderer::BeginFrame() const {
  imGuiInstance.BeginFrame();
}

void UiRenderer::EndFrame() const {
  imGuiInstance.RenderFrame();
}

void UiRenderer::Render(const CommandBuffer& commandBuffer) const {
  imGuiInstance.SubmitFrame(commandBuffer);
}

void UiRenderer::ShowVulkanDebugInfo(const VulkanDebugInfo& info) {
  timeSum += info.frametime;

  ImGui::SetNextWindowPos(ImVec2(Margin, Margin), ImGuiCond_Once);
  ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
  ImGui::Begin("Vulkan Debug", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
  ImGui::Text(info.gpuName);

  const float frametimeMs = info.frametime * 1000.0f;

  if (timeSum > 1.0f) {
    if (valuesFilled < 10) {
      ++valuesFilled;
    }
    frametimeHistory[frametimeHistoryIndex] = frametimeMs;
    frametimeHistoryIndex =
        (frametimeHistoryIndex + 1) % frametimeHistory.size();
    timeSum = 0.0f;
  }

  const float averageFrametime = Average(frametimeHistory.data(), valuesFilled);

  ImGui::Text("Frametime: %.3fms", averageFrametime);
  ImGui::PlotLines("frametimes",
                   &UiRenderer::GetFrametimeHistoryValueFromRenderer, this,
                   frametimeHistory.size());
  ImGui::Text("FPS: %.0f", 1000.0f / averageFrametime);
  ImGui::End();
}

float UiRenderer::GetFrametimeHistoryValueFromRenderer(void* data, int index) {
  return reinterpret_cast<const UiRenderer*>(data)->GetFrametimeHistoryValue(
      nullptr, index);
}

float UiRenderer::GetFrametimeHistoryValue(void* data, int index) const {
  return frametimeHistory[(frametimeHistoryIndex + index) %
                          frametimeHistory.size()];
}
