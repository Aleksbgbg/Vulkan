#ifndef VULKAN_SRC_TEXTRENDERER_H
#define VULKAN_SRC_TEXTRENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "types.h"

#define FT_PROCEED_ON_SUCCESS(statement)        \
  if (statement) {                              \
    throw std::runtime_error("FreeType error"); \
  }

void RenderText(const std::string text, void* memory, const u32 width, const u32 height) {
  FT_Library library;
  FT_PROCEED_ON_SUCCESS(FT_Init_FreeType(&library))

  FT_Face fontFace;
  FT_PROCEED_ON_SUCCESS(FT_New_Face(library, "SEGOEUI.TTF", 0, &fontFace))

  FT_PROCEED_ON_SUCCESS(FT_Set_Char_Size(fontFace, 0, 64 * 64, 96, 96))

  for (u32 i = 0; i < text.size(); ++i) {
    const u32 glyphIndex = FT_Get_Char_Index(fontFace, text[i]);

    FT_PROCEED_ON_SUCCESS(FT_Load_Glyph(fontFace, glyphIndex, FT_LOAD_DEFAULT))

    if (fontFace->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
      FT_PROCEED_ON_SUCCESS(FT_Render_Glyph(fontFace->glyph, FT_RENDER_MODE_NORMAL))
    }

    for (u32 x = 0; x < fontFace->glyph->bitmap.width; ++x) {
      for (u32 y = 0; y < fontFace->glyph->bitmap.rows; ++y) {
        u8 value = fontFace->glyph->bitmap.buffer[x + (fontFace->glyph->bitmap.pitch * y)];

        if (value == 0) {
          continue;
        }

        reinterpret_cast<u8*>(memory)[(((64 * i) + x + (width * y)) * 4) + 0] = value;
        reinterpret_cast<u8*>(memory)[(((64 * i) + x + (width * y)) * 4) + 1] = value;
        reinterpret_cast<u8*>(memory)[(((64 * i) + x + (width * y)) * 4) + 2] = value;
        reinterpret_cast<u8*>(memory)[(((64 * i) + x + (width * y)) * 4) + 3] = value;
      }
    }
  }

//  for (u32 y = 0; y < fontFace->glyph->bitmap.rows; ++y) {
//    for (u32 x = 0; x < fontFace->glyph->bitmap.width; ++x) {
//      u32 p1 = x + (width * y);
//      u32 p2 = x + (fontFace->glyph->bitmap.pitch * y);
//      reinterpret_cast<u8*>(memory)[p1 + 0] = 0; //fontFace->glyph->bitmap.buffer[p2];
//      reinterpret_cast<u8*>(memory)[p1 + 1] = 0;//fontFace->glyph->bitmap.buffer[p2];
//      reinterpret_cast<u8*>(memory)[p1 + 2] = 0;//fontFace->glyph->bitmap.buffer[p2];
//      // reinterpret_cast<u8*>(memory)[x + (width * y) + 3] = fontFace->glyph->bitmap.buffer[x + (fontFace->glyph->bitmap.pitch * y)];
//    }
//  }
}

#endif // VULKAN_SRC_TEXTRENDERER_H
