#include "DebugViewDrawList.h"

#include <iomanip>
#include <sstream>

#include "engine/RenderPerformanceTracker.h"
#include "engine/ui/Canvas.h"

namespace {

template <typename T>
constexpr T NormalizedToNdc(const T value) {
  return (value * 2.0f) - 1.0f;
}

}  // namespace

DebugViewDrawList::DebugViewDrawList(
    const FontAtlas& fontAtlas,
    const RenderPerformanceTracker& renderPerformanceTracker)
    : fontAtlas_(fontAtlas),
      renderPerformanceTracker_(renderPerformanceTracker) {}

DrawList::DrawData DebugViewDrawList::Generate(const glm::vec2 windowSize) {
  DrawVertices<PositionColorVertex> elementVertices;
  DrawVertices<TextVertex> textVertices;

  Canvas canvas(fontAtlas_, elementVertices.vertices, textVertices.vertices);

  std::ostringstream fpsString;
  fpsString << "FPS: " << std::floor(renderPerformanceTracker_.GetFps());

  canvas.Write(Rectu::FromPoints(0, 0, windowSize.x, canvas.GetTextHeight()),
               fpsString.str(), TextCase::Regular, TextAlignment::Left, WHITE);

  // Required to feed renderer some empty vertices
  canvas.DrawRect(Rectu(), BLACK);

  for (u32 index = 0; index < elementVertices.vertices.size(); ++index) {
    PositionColorVertex& vertex = elementVertices.vertices[index];
    vertex.position = NormalizedToNdc(
        glm::vec3(glm::vec2(vertex.position) / windowSize, 0.0f));
    elementVertices.indices.push_back(index);
  }
  for (u32 index = 0; index < textVertices.vertices.size(); ++index) {
    TextVertex& vertex = textVertices.vertices[index];
    vertex.position = NormalizedToNdc(
        glm::vec3(glm::vec2(vertex.position) / windowSize, 0.0f));
    textVertices.indices.push_back(index);
  }

  return {.elements = std::make_unique<VertexDataOfType<PositionColorVertex>>(
              std::move(elementVertices)),
          .text = std::make_unique<VertexDataOfType<TextVertex>>(
              std::move(textVertices))};
}
