#ifndef SRC_ENGINE_UI_DRAWLIST_H_
#define SRC_ENGINE_UI_DRAWLIST_H_

#include <memory>

#include "engine/resource/Resource.h"
#include "renderer/vertices/StructuredVertexData.h"

class DrawList : public Resource {
 public:
  virtual ~DrawList() = default;

  struct DrawData {
    std::unique_ptr<StructuredVertexData> elements;
    std::unique_ptr<StructuredVertexData> text;
  };
  virtual DrawList::DrawData Generate(glm::vec2 windowSize) = 0;
};

#endif  // SRC_ENGINE_UI_DRAWLIST_H_
