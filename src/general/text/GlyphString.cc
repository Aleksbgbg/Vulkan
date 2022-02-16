#include "GlyphString.h"

UniformGlyphString::UniformGlyphString(const std::string& string)
    : string_(string.begin(), string.end()) {}

UniformGlyphString::UniformGlyphString(std::string_view string)
    : string_(string.begin(), string.end()) {}

UniformGlyphString::UniformGlyphString(const GlyphString string)
    : string_(string) {}

GlyphString UniformGlyphString::AsGlyphString() const {
  return string_;
}

GlyphStringView UniformGlyphString::AsGlyphStringView() const {
  return string_;
}
