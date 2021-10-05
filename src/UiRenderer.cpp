#include "UiRenderer.h"

#include <format>
#include <string>

#include <imgui.h>

#include "build_definition.h"

float Average(const float* const values, const u32 size) {
  float sum = 0.0f;

  for (u32 index = 0; index < size; ++index) {
    sum += values[index];
  }

  return sum / size;
}

static constexpr float ButtonSize = 50.0f;

UiRenderer::UiRenderer(ImGuiInstance imGuiInstance)
  :
  imGuiInstance(std::move(imGuiInstance)),
  valuesFilled(0),
  frametimeHistoryIndex(0),
  frametimeHistory(),
  timeSum(0.0f),
  spawnCubeKey(KeyDescription("F", SDLK_f, "spawn cube")),
  cameraKeys({
    KeyDescription("\xEF\x84\x86", SDLK_UP, "pivot camera up"),
    KeyDescription("\xEF\x84\x84", SDLK_LEFT, "pivot camera left"),
    KeyDescription("\xEF\x84\x87", SDLK_DOWN, "pivot camera down"),
    KeyDescription("\xEF\x84\x85", SDLK_RIGHT, "pivot camera right" )})
{
  sceneObjectControls[0].emplace_back(KeyDescription("Q", SDLK_q, "decrease Z"));
  sceneObjectControls[0].emplace_back(KeyDescription("W", SDLK_w, "increase Y"));
  sceneObjectControls[0].emplace_back(KeyDescription("E", SDLK_e, "increase Z"));
  sceneObjectControls[1].emplace_back(KeyDescription("A", SDLK_a, "decrease X"));
  sceneObjectControls[1].emplace_back(KeyDescription("S", SDLK_s, "decrease Y"));
  sceneObjectControls[1].emplace_back(KeyDescription("D", SDLK_d, "increase X"));
}

void UiRenderer::ProcessEvent(const SDL_Event& event) const {
  imGuiInstance.ProcessEvent(event);
}

void UiRenderer::BeginFrame() const {
  imGuiInstance.BeginFrame();
}

void UiRenderer::EndFrame() const {
#ifdef TOOLS
  ImGui::ShowDemoWindow();
#endif

  imGuiInstance.RenderFrame();
}

void UiRenderer::Render(const CommandBuffer& commandBuffer) const {
  imGuiInstance.SubmitFrame(commandBuffer);
}

void UiRenderer::ShowVulkanDebugInfo(const VulkanDebugInfo& info) {
  timeSum += info.frametime;

  ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f));
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
  ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f));
  ImGui::Begin("Objects in Scene");
  ImGui::Text("Camera positioned at (%.3f, %.3f, %.3f)", info.cameraPosition.x, info.cameraPosition.y, info.cameraPosition.z);
  ImGui::Text("Camera looking at (%.3f, %.3f, %.3f)", info.cameraCenter.x, info.cameraCenter.y, info.cameraCenter.z);
  ImGui::Text("Camera rotated by (%.1f°, %.1f°)", info.cameraRotation.x, info.cameraRotation.y);
  ImGui::BeginTabBar("Cubes", ImGuiTabBarFlags_AutoSelectNewTabs);
  for (u32 cubeIndex = 0; cubeIndex < info.cubePositions.size(); ++cubeIndex) {
    if (ImGui::BeginTabItem(std::format("Cube {}", cubeIndex + 1).c_str())) {
      glm::vec3* value = info.cubePositions[cubeIndex];

      ImGui::InputFloat("X", &value->x, 0.0f, 0.0f, "%.3f");
      ImGui::InputFloat("Y", &value->y, 0.0f, 0.0f, "%.3f");
      ImGui::InputFloat("Z", &value->z, 0.0f, 0.0f, "%.3f");
      ImGui::EndTabItem();

      (*info.selectedObjectIndex) = cubeIndex;
    }
  }
  ImGui::EndTabBar();
  ImGui::End();
}

void UiRenderer::RenderKey(Keyboard& keyboard, KeyDescription& key) {
  const bool isKeyDown = keyboard.IsKeyDown(key.code); // Race condition if not saved

  if (isKeyDown) {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
  }
  ImGui::Button(key.key, ImVec2(ButtonSize, ButtonSize));
  if (ImGui::IsItemActive()) {
    keyboard.Keydown(key.code);
    key.pressed = true;
  } else if (key.pressed) {
    keyboard.Keyup(key.code);
    key.pressed = false;
  }
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip(key.description);
  }
  if (isKeyDown) {
    ImGui::PopStyleColor();
  }
}

void UiRenderer::ShowKeyboardLayout(Keyboard& keyboard) {
  ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f));
  ImGui::Begin("Keyboard Layout");

  ImGui::Text("Move selected scene object");
  for (std::vector<KeyDescription>& row : sceneObjectControls) {
    for (u32 keyIndex = 0; keyIndex < (row.size() - 1); ++keyIndex) {
      KeyDescription& key = row[keyIndex];
      RenderKey(keyboard, key);

      ImGui::SameLine();
    }
    RenderKey(keyboard, row[row.size() - 1]);
  }

  ImGui::Text("Pivot camera");
  ImGui::Dummy(ImVec2(ButtonSize, ButtonSize));
  ImGui::SameLine();
  RenderKey(keyboard, cameraKeys[0]);
  RenderKey(keyboard, cameraKeys[1]);
  ImGui::SameLine();
  RenderKey(keyboard, cameraKeys[2]);
  ImGui::SameLine();
  RenderKey(keyboard, cameraKeys[3]);

  ImGui::Text("Spawn cube");
  RenderKey(keyboard, spawnCubeKey);

  ImGui::End();
}
