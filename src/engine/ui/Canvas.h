#ifndef SRC_ENGINE_UI_CANVAS_H_
#define SRC_ENGINE_UI_CANVAS_H_

#include <vector>

#include "Color.h"
#include "TextAlignment.h"
#include "TextCase.h"
#include "core/math/Rect.h"
#include "core/text/FontAtlas.h"
#include "core/text/GlyphString.h"
#include "renderer/vertices/PositionColorVertex.h"
#include "renderer/vertices/TextVertex.h"

inline constexpr Color BLACK_BACKGROUND = {0.0f, 0.0f, 0.0f, 0.5f};

class Canvas {
 public:
  Canvas(const FontAtlas& fontAtlas,
         std::vector<PositionColorVertex>& elementVertices,
         std::vector<TextVertex>& textVertices);

  u32 GetTextHeight() const;

  void DrawRect(Rectu renderRegion, Color color) const;
  void Write(Rectu region, UniformGlyphString string, TextCase textCase,
             TextAlignment alignment, Color color) const;

 private:
  const FontAtlas& fontAtlas_;
  std::vector<PositionColorVertex>& elementVertices_;
  std::vector<TextVertex>& textVertices_;
};

#endif  // SRC_ENGINE_UI_CANVAS_H_
