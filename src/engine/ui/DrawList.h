#ifndef VULKAN_SRC_ENGINE_UI_DRAWLIST_H_
#define VULKAN_SRC_ENGINE_UI_DRAWLIST_H_

#include <memory>

#include "renderer/vertices/StructuredVertexData.h"

class DrawList {
 public:
  virtual ~DrawList() = default;

  struct DrawData {
    std::unique_ptr<StructuredVertexData> elements;
    std::unique_ptr<StructuredVertexData> text;
  };
  virtual DrawList::DrawData Generate(glm::vec2 windowSize) = 0;
};

#endif  // VULKAN_SRC_ENGINE_UI_DRAWLIST_H_
