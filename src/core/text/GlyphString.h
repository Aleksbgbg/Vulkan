#ifndef VULKAN_SRC_CORE_TEXT_GLYPHSTRING_H_
#define VULKAN_SRC_CORE_TEXT_GLYPHSTRING_H_

#include <string>
#include <string_view>

using GlyphCode = char16_t;
using GlyphString = std::basic_string<GlyphCode>;
using GlyphStringView = std::basic_string_view<GlyphCode>;

class UniformGlyphString {
 public:
  UniformGlyphString(const std::string& string);
  UniformGlyphString(std::string_view string);
  UniformGlyphString(GlyphString string);
  inline UniformGlyphString(const GlyphCode* const string)
      : string_(string, string + std::char_traits<GlyphCode>::length(string)) {}

  GlyphString AsGlyphString() const;
  GlyphStringView AsGlyphStringView() const;

 private:
  GlyphString string_;
};

#define GLYPH_STRING_LITERAL(LITERAL) u##LITERAL
#define TO_GLYPH_STRING(STRING) UniformGlyphString(STRING).AsGlyphString()

#endif  // VULKAN_SRC_CORE_TEXT_GLYPHSTRING_H_
