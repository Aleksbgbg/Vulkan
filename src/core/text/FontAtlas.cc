#include "FontAtlas.h"

#include <cassert>
#include <list>

#include "core/adapters/MapValueIterator.h"
#include "util/filenames.h"
#include "util/include/freetype.h"

#ifdef DEBUG
#define PROCEED_ON_VALID_RESULT(CALL) assert(CALL == 0)
#else
#define PROCEED_ON_VALID_RESULT(CALL) CALL
#endif

using GlyphList = std::list<GlyphCode>;

struct FontFile {
  std::string_view filename;
  GlyphList glyphs;
};

void AddGlyph(GlyphList& list, const GlyphCode glyph) {
  list.push_back(glyph);
}

void AddGlyphRange(GlyphList& list, const GlyphCode start,
                   const GlyphCode end) {
  for (GlyphCode glyph = start; glyph <= end; ++glyph) {
    AddGlyph(list, glyph);
  }
}

FontAtlas FontAtlas::Create() {
  FT_Library library;
  PROCEED_ON_VALID_RESULT(FT_Init_FreeType(&library));

  std::list<FontFile> files;
  FontFile ui;
  ui.filename = UI_FONT_FILENAME;
  AddGlyphRange(ui.glyphs, ' ', 'z');
  FontFile fontawesome;
  fontawesome.filename = FONTAWESOME_FONT_FILENAME;
  AddGlyphRange(fontawesome.glyphs, 61700, 61701);

  files.push_back(std::move(ui));
  files.push_back(std::move(fontawesome));

  std::unordered_map<GlyphCode, Bitmap> glyphImages;
  u32 totalWidth = 0;
  u32 maxHeight = 0;

  std::unordered_map<GlyphCode, Glyph> glyphs;

  u32 fontHeight = 0;

  for (const FontFile& fontFile : files) {
    FT_Face face;
    PROCEED_ON_VALID_RESULT(
        FT_New_Face(library, fontFile.filename.data(), 0, &face));

    PROCEED_ON_VALID_RESULT(
        FT_Set_Char_Size(face,    /* handle to face object           */
                         0,       /* char_width in 1/64th of points  */
                         18 * 64, /* char_height in 1/64th of points */
                         96,      /* horizontal device resolution    */
                         0));     /* vertical device resolution      */
    // PROCEED_ON_VALID_RESULT(FT_Set_Pixel_Sizes(face, 20, 0));

    fontHeight = std::max<u32>(fontHeight, face->height);
    FT_GlyphSlot slot = face->glyph;

    for (const GlyphCode charCode : fontFile.glyphs) {
      auto glyphIndex = FT_Get_Char_Index(face, charCode);
      PROCEED_ON_VALID_RESULT(FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT));
      PROCEED_ON_VALID_RESULT(FT_Outline_Embolden(&slot->outline, 32));
      PROCEED_ON_VALID_RESULT(
          FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL));

      const FT_Bitmap glyph = slot->bitmap;

      Bitmap& image = glyphImages[charCode];
      image = Bitmap(glyph.width, glyph.rows, glyph.pitch, glyph.buffer,
                     BytesPerPixel(1));

      Glyph& glyphData = glyphs[charCode];
      glyphData.width = glyph.width;
      glyphData.height = glyph.rows;
      glyphData.advance = slot->advance.x / 64;
      glyphData.bearing = glm::vec2(slot->bitmap_left, slot->bitmap_top);

      totalWidth += image.Width();
      maxHeight = std::max(maxHeight, image.Height());
    }
  }

  u32 currentWidth = 0;
  for (const auto& pair : glyphImages) {
    const GlyphCode charCode = pair.first;
    const Bitmap& image = pair.second;

    Glyph& glyphData = glyphs[charCode];
    glyphData.textureCoordinate = Rectf::FromPoints(
        static_cast<float>(currentWidth) / totalWidth, 0.0f,
        static_cast<float>(currentWidth + glyphData.width) / totalWidth,
        static_cast<float>(glyphData.height) / maxHeight);
    currentWidth += image.Width();
  }

  Bitmap atlas(totalWidth, maxHeight, BytesPerPixel(1));

  u32 widthAlong = 0;
  for (const Bitmap& image : IterateValues(glyphImages)) {
    Bitmap::Copy(image, atlas, Offset(widthAlong, 0));
    widthAlong += image.Width();
  }

  return FontAtlas(fontHeight / 64, std::move(atlas), std::move(glyphs));
}

FontAtlas::FontAtlas(const u32 fontHeight, Bitmap image,
                     std::unordered_map<GlyphCode, Glyph> glyphs)
    : fontHeight_(fontHeight),
      image_(std::move(image)),
      glyphs_(std::move(glyphs)) {}

u32 FontAtlas::GetFontHeight() const {
  return fontHeight_;
}

const Bitmap& FontAtlas::AsImage() const {
  return image_;
}

const Glyph& FontAtlas::RetrieveGlyph(const GlyphCode charCode) const {
  if (!glyphs_.contains(charCode)) {
    return FirstValue(glyphs_);
  }

  return glyphs_.at(charCode);
}
