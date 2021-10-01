#include "UiRenderer.h"

#include <format>
#include <string>

#include <imgui.h>

float Average(const float* const values, const u32 size) {
  float sum = 0.0f;

  for (u32 index = 0; index < size; ++index) {
    sum += values[index];
  }

  return sum / size;
}

UiRenderer::UiRenderer(ImGuiInstance imGuiInstance)
  :
  imGuiInstance(std::move(imGuiInstance)),
  valuesFilled(0),
  frametimeHistoryIndex(0),
  frametimeHistory(),
  timeSum(0.0f)
{
  sceneObjectControls[0].emplace_back(KeyDescription{ .key = "Q", .code = SDLK_q, .description = "decrease Z" });
  sceneObjectControls[0].emplace_back(KeyDescription{ .key = "W", .code = SDLK_w, .description = "increase Y" });
  sceneObjectControls[0].emplace_back(KeyDescription{ .key = "E", .code = SDLK_e, .description = "increase Z" });
  sceneObjectControls[1].emplace_back(KeyDescription{ .key = "A", .code = SDLK_a, .description = "decrease X" });
  sceneObjectControls[1].emplace_back(KeyDescription{ .key = "S", .code = SDLK_s, .description = "decrease Y" });
  sceneObjectControls[1].emplace_back(KeyDescription{ .key = "D", .code = SDLK_d, .description = "increase X" });
}

void UiRenderer::ProcessEvent(const SDL_Event& event) const {
  imGuiInstance.ProcessEvent(event);
}

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

  ImGui::Begin("Vulkan Debug");
  ImGui::Text(info.gpuName);

  const float frametimeMs = info.frametime * 1000.0f;

  if (timeSum > 1.0f) {
    if (valuesFilled < 10) {
      ++valuesFilled;
    }
    frametimeHistory[frametimeHistoryIndex] = frametimeMs;
    frametimeHistoryIndex = (frametimeHistoryIndex + 1) % frametimeHistory.size();
    timeSum = 0.0f;
  }

  const float averageFrametime = Average(frametimeHistory.data(), valuesFilled);

  ImGui::Text("Frametime: %.3fms", averageFrametime);
  ImGui::PlotLines("frametimes", &UiRenderer::GetFrametimeHistoryValueFromRenderer, this, frametimeHistory.size());
  ImGui::Text("FPS: %.0f", 1000.0f / averageFrametime);
  ImGui::End();
}

float UiRenderer::GetFrametimeHistoryValueFromRenderer(void* data, int index) {
  return reinterpret_cast<const UiRenderer*>(data)->GetFrametimeHistoryValue(nullptr, index);
}

float UiRenderer::GetFrametimeHistoryValue(void* data, int index) const {
  return frametimeHistory[(frametimeHistoryIndex + index) % frametimeHistory.size()];
}

void UiRenderer::ShowObjectsInScene(const ObjectsInSceneInfo& info) const {
  ImGui::ShowDemoWindow();

  ImGui::Begin("Objects in Scene");
  ImGui::Text("Camera positioned at (%.3f, %.3f, %.3f)", info.cameraPosition.x, info.cameraPosition.y, info.cameraPosition.z);
  ImGui::Text("Camera looking at (%.3f, %.3f, %.3f)", info.cameraCenter.x, info.cameraCenter.y, info.cameraCenter.z);
  ImGui::Text("Camera rotated by (%.1f°, %.1f°)", info.cameraRotation.x, info.cameraRotation.y);
  ImGui::BeginTabBar("Cubes", ImGuiTabBarFlags_AutoSelectNewTabs);
  u32 i = 0;
  for (glm::vec3* value : info.cubePositions) {
    if (ImGui::BeginTabItem(std::format("Cube {}", ++i).c_str())) {
      ImGui::InputFloat("X", &value->x, 0.0f, 0.0f, "%.3f");
      ImGui::InputFloat("Y", &value->y, 0.0f, 0.0f, "%.3f");
      ImGui::InputFloat("Z", &value->z, 0.0f, 0.0f, "%.3f");
      ImGui::EndTabItem();
    }
  }
  ImGui::EndTabBar();
  ImGui::End();
}

void UiRenderer::RenderKey(const Keyboard& keyboard, const KeyDescription& key) {
  if (keyboard.IsKeyDown(key.code)) {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
  }
  ImGui::Button(key.key, ImVec2(25.0f, 25.0f));
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip(key.description);
  }
  if (keyboard.IsKeyDown(key.code)) {
    ImGui::PopStyleColor();
  }
}

void UiRenderer::ShowKeyboardLayout(const Keyboard& keyboard) const {
  ImGui::Begin("Keyboard Layout");

  ImGui::Text("Move selected scene object");
  for (const std::vector<KeyDescription>& row : sceneObjectControls) {
    for (u32 keyIndex = 0; keyIndex < (row.size() - 1); ++keyIndex) {
      const KeyDescription& key = row[keyIndex];
      RenderKey(keyboard, key);

      ImGui::SameLine();
    }
    RenderKey(keyboard, row[row.size() - 1]);
  }

  ImGui::Text("Pivot camera");
  ImGui::Dummy(ImVec2(25.0f, 25.0f));
  ImGui::SameLine();
  RenderKey(keyboard, KeyDescription{ .key = "\xEF\x84\x86", .code = SDLK_UP, .description = "pivot camera up" });
  RenderKey(keyboard, KeyDescription{ .key = "\xEF\x84\x84", .code = SDLK_LEFT, .description = "pivot camera left" });
  ImGui::SameLine();
  RenderKey(keyboard, KeyDescription{ .key = "\xEF\x84\x87", .code = SDLK_DOWN, .description = "pivot camera down" });
  ImGui::SameLine();
  RenderKey(keyboard, KeyDescription{ .key = "\xEF\x84\x85", .code = SDLK_RIGHT, .description = "pivot camera right" });

  ImGui::Text("Spawn cube");
  RenderKey(keyboard, KeyDescription{ .key = "F", .code = SDLK_f, .description = "spawn cube" });

  ImGui::End();
}
