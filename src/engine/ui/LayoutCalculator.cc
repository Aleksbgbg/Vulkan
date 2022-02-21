#include "LayoutCalculator.h"

namespace {

static constexpr u32 ROW_MARGIN = 2;

}  // namespace

u32 TextHeight(const FontAtlas& fontAtlas) {
  static constexpr u32 TextVerticalMargin = 5;
  return fontAtlas.GetFontHeight() + (2 * TextVerticalMargin);
}

LayoutCalculator::LayoutCalculator(const FontAtlas& fontAtlas,
                                   const Rectu layoutArea)
    : fontAtlas_(fontAtlas), layoutArea_(layoutArea) {}

LayoutCalculator LayoutCalculator::Partition(const Rectu area) const {
  return LayoutCalculator(fontAtlas_, area);
}

ListBoxLayout LayoutCalculator::CalculateLayout(const ListBox& listBox) const {
  const Rectu margin = listBox.margin.fractional * layoutArea_.Size();
  const Rectu layoutArea =
      Rectu::FromPoints(margin.Point1() + layoutArea_.Point1(),
                        layoutArea_.Point2() - margin.Point2());

  const u32 headerHeight = TextHeight(fontAtlas_);

  ListBoxLayout layout;

  if (listBox.orientation == Orientation::Horizontal) {
    static constexpr u32 AccentHeight = 10;
    static constexpr u32 HeaderWidth = 400;
    static constexpr u32 TabMargin = 2;
    static constexpr u32 ContentMargin = 30;

    layout.headerArea =
        Rectu::FromRegion(layoutArea.X1(), layoutArea.Y1() + AccentHeight,
                          layoutArea.Width(), headerHeight);
    layout.contentArea = Rectu::FromPoints(
        layout.headerArea.X1(), layout.headerArea.Y2() + ContentMargin,
        layoutArea.Point2());

    layout.headers.reserve(listBox.elements.size());
    for (std::size_t index = 0; index < listBox.elements.size(); ++index) {
      const u32 distanceAcross = index * (HeaderWidth + TabMargin);

      const ContainerElement<ListBoxElement>& element =
          listBox.elements.at(index);

      std::optional<Rectu> accent;
      if (element.IsSelected()) {
        accent = Rectu::FromPoints(
            layout.headerArea.X1() + distanceAcross, layoutArea.Y1(),
            layout.headerArea.X1() + distanceAcross + HeaderWidth,
            layout.headerArea.Y1());
      }
      layout.headers.push_back(
          {.accent = accent,
           .area = Rectu::FromPoints(
               layout.headerArea.X1() + distanceAcross, layout.headerArea.Y1(),
               layout.headerArea.X1() + distanceAcross + HeaderWidth,
               layout.headerArea.Y2())});
    }
  } else {
    static constexpr u32 HeaderWidth = 700;
    static constexpr u32 ContentAreaMargin = 2 * ROW_MARGIN;

    layout.headerArea = Rectu::FromRegion(layoutArea.Point1(), HeaderWidth,
                                          layoutArea.Height());
    layout.contentArea =
        Rectu::FromPoints(layout.headerArea.X2() + ContentAreaMargin,
                          layout.headerArea.Y1(), layoutArea.Point2());

    layout.headers.reserve(listBox.elements.size());
    for (u32 index = 0; index < listBox.elements.size(); ++index) {
      const u32 distanceDown = index * (headerHeight + (2 * ROW_MARGIN));
      layout.headers.push_back(
          {.accent = std::nullopt,
           .area = Rectu::FromPoints(
               layout.headerArea.X1(), layout.headerArea.Y1() + distanceDown,
               layout.headerArea.X2(),
               layout.headerArea.Y1() + distanceDown + headerHeight)});
    }
  }

  return layout;
}

GridListLayout LayoutCalculator::CalculateLayout(
    const GridList& gridList) const {
  static constexpr u32 HorizontalMargin = 10;
  const u32 elementHeight = TextHeight(fontAtlas_);

  GridListLayout layout;

  for (u32 index = 0; index < gridList.elements.size(); ++index) {
    const u32 position = index * (elementHeight + (2 * ROW_MARGIN));

    const Rectu rowArea = Rectu::FromPoints(
        layoutArea_.X1(), layoutArea_.Y1() + position, layoutArea_.X2(),
        layoutArea_.Y1() + position + elementHeight);

    constexpr float TitleAreaRatio = 3.0f / 4.0f;
    layout.rows.push_back(
        {.rowArea = rowArea,
         .titleArea = Rectu::FromPoints(
             HorizontalMargin + rowArea.X1(), rowArea.Y1(),
             rowArea.X1() + (rowArea.Width() * TitleAreaRatio), rowArea.Y2()),
         .contentArea = Rectu::FromPoints(
             rowArea.X1() + (rowArea.Width() * TitleAreaRatio), rowArea.Y1(),
             rowArea.X2() - HorizontalMargin, rowArea.Y2())});
  }

  return layout;
}

ToggleBoxLayout LayoutCalculator::CalculateLayout(
    const ToggleBox& toggleBox) const {
  return {.renderArea = layoutArea_};
}

SliderLayout LayoutCalculator::CalculateLayout(const Slider& slider) const {
  static constexpr u32 Height = 10;

  const u32 top = layoutArea_.Y1() + (layoutArea_.Height() / 2) - (Height / 2);

  const u32 width = layoutArea_.Width();
  const u32 segment1Width = width * slider.value;
  const u32 segment2Width = width - segment1Width;

  SliderLayout layout;
  layout.renderArea = layoutArea_;
  layout.background =
      Rectu::FromRegion(layoutArea_.X1(), top, segment1Width, Height);
  layout.highlight = Rectu::FromRegion(layoutArea_.X1() + segment1Width, top,
                                       segment2Width, Height);
  return layout;
}
