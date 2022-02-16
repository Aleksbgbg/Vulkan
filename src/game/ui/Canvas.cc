#include "Canvas.h"

constexpr GlyphCode ToUpper(const GlyphCode character) {
  if ((97 <= character) && (character <= 122)) {
    return character - 32;
  }

  return character;
}

GlyphString ConvertTextCase(const GlyphStringView text,
                            const TextCase textCase) {
  switch (textCase) {
    default:
    case TextCase::Regular:
      return text.data();

    case TextCase::Uppercase: {
      GlyphString result;

      for (const GlyphCode character : text) {
        result += ToUpper(character);
      }

      return result;
    }
  }
}

struct TextMetrics {
  glm::uvec2 size;
  glm::uvec2 maxBearing;
};

TextMetrics CalculateTextMetrics(const FontAtlas& fontAtlas,
                                 const GlyphStringView text) {
  TextMetrics metrics{};

  for (const GlyphCode character : text) {
    const Glyph& glyph = fontAtlas.RetrieveGlyph(character);

    metrics.size.x += glyph.advance;
    metrics.size.y = std::max(metrics.size.y, glyph.height);

    metrics.maxBearing.x = std::max<u32>(metrics.maxBearing.x, glyph.bearing.x);
    metrics.maxBearing.y = std::max<u32>(metrics.maxBearing.y, glyph.bearing.y);
  }

  return metrics;
}

Canvas::Canvas(const FontAtlas& fontAtlas,
               std::vector<PositionColorVertex>& elementVertices,
               std::vector<TextVertex>& textVertices)
    : fontAtlas_(fontAtlas),
      elementVertices_(elementVertices),
      textVertices_(textVertices) {}

u32 Canvas::GetTextHeight() const {
  return fontAtlas_.GetFontHeight();
}

void Canvas::DrawRect(const Rectu renderRegion, const Color color) const {
  const glm::vec4 renderColor(color.r, color.g, color.b, color.a);

  elementVertices_.push_back(
      {{renderRegion.X1(), renderRegion.Y1(), 0.0f}, renderColor});
  for (u32 i = 0; i < 2; ++i) {
    elementVertices_.push_back(
        {{renderRegion.X2(), renderRegion.Y1(), 0.0f}, renderColor});
    elementVertices_.push_back(
        {{renderRegion.X1(), renderRegion.Y2(), 0.0f}, renderColor});
  }
  elementVertices_.push_back(
      {{renderRegion.X2(), renderRegion.Y2(), 0.0f}, renderColor});
}

void Canvas::Write(const Rectu region, const UniformGlyphString string,
                   const TextCase textCase, const TextAlignment alignment,
                   const Color color) const {
  const glm::vec3 colorVertex = glm::vec3(color.r, color.g, color.b);
  const GlyphString text =
      ConvertTextCase(string.AsGlyphStringView(), textCase);
  const TextMetrics metrics = CalculateTextMetrics(fontAtlas_, text);

  const u32 baseline = (region.Height() / 2.0f) +
                       (metrics.maxBearing.y - (metrics.size.y / 2.0f));

  u32 xOffset = 0;

  if (alignment == TextAlignment::Center) {
    xOffset = (region.Width() - metrics.size.x) / 2.0f;
  } else if (alignment == TextAlignment::Right) {
    xOffset = region.Width() - metrics.size.x;
  }

  u32 position = 0.0f;

  for (const GlyphCode character : text) {
    const Glyph& glyph = fontAtlas_.RetrieveGlyph(character);

    const u32 left = xOffset + position + glyph.bearing.x;
    const u32 top = baseline - glyph.bearing.y;

    const Rectf renderRegion =
        Rectu::FromPoints(region.Point(), region.Point()) +
        Rectu::FromPoints(left, top, left + glyph.width, top + glyph.height);

    const Rectf& textureCoordinate = glyph.textureCoordinate;

    textVertices_.push_back({{renderRegion.X1(), renderRegion.Y1(), 0.0f},
                             {textureCoordinate.X1(), textureCoordinate.Y1()},
                             colorVertex});
    for (u32 i = 0; i < 2; ++i) {
      textVertices_.push_back({{renderRegion.X2(), renderRegion.Y1(), 0.0f},
                               {textureCoordinate.X2(), textureCoordinate.Y1()},
                               colorVertex});
      textVertices_.push_back({{renderRegion.X1(), renderRegion.Y2(), 0.0f},
                               {textureCoordinate.X1(), textureCoordinate.Y2()},
                               colorVertex});
    }
    textVertices_.push_back({{renderRegion.X2(), renderRegion.Y2(), 0.0f},
                             {textureCoordinate.X2(), textureCoordinate.Y2()},
                             colorVertex});

    position += glyph.advance;
  }
}
