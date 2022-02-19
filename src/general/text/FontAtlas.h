#ifndef VULKAN_SRC_GENERAL_TEXT_FONTATLAS_H_
#define VULKAN_SRC_GENERAL_TEXT_FONTATLAS_H_

#include <unordered_map>

#include "GlyphString.h"
#include "general/geometry/Rect.h"
#include "general/images/Bitmap.h"
#include "util/include/glm.h"
#include "util/types.h"

struct Glyph {
  u32 width;
  u32 height;
  u32 advance;
  glm::vec2 bearing;
  Rectf textureCoordinate;
};

class FontAtlas {
 public:
  static FontAtlas Create();

  u32 GetFontHeight() const;
  const Bitmap& AsImage() const;
  const Glyph& RetrieveGlyph(GlyphCode charCode) const;

 private:
  FontAtlas(u32 fontHeight, Bitmap image,
            std::unordered_map<GlyphCode, Glyph> glyphs);

 private:
  u32 fontHeight_;
  Bitmap image_;
  std::unordered_map<GlyphCode, Glyph> glyphs_;
};

#endif  // VULKAN_SRC_GENERAL_TEXT_FONTATLAS_H_
