#include "UiDrawList.h"

#include "UiElement.h"
#include "produce_ui_tree.h"

namespace {

template <typename T>
constexpr T NormalizedToNdc(const T value) {
  return (value * 2.0f) - 1.0f;
}

}  // namespace

UiDrawList::UiDrawList(const FontAtlas& fontAtlas,
                       GraphicalInterface& interface, UiTree uiTree,
                       glm::vec2 windowSize)
    : fontAtlas_(fontAtlas),
      uiTree_(std::move(uiTree)),
      interface_(interface),
      focusTracker_(uiTree_),
      events_(),
      draw_(),
      text_() {
  FocusRoot();
  CalculateLayout(windowSize);
}

void UiDrawList::Consume(const SDL_Event& event) {
  if (interface_.IsVisible()) {
    events_.push(event);
  }
}

DrawList::DrawData UiDrawList::Generate(const glm::vec2 windowSize) {
  CalculateLayout(windowSize);
  Render(windowSize);

  return {
      .elements = std::make_unique<VertexDataOfType<PositionColorVertex>>(
          std::move(draw_)),
      .text = std::make_unique<VertexDataOfType<TextVertex>>(std::move(text_))};
}

void UiDrawList::UpdateModel(const UpdateContext& context) {
  ProcessEvents();
}

void UiDrawList::ProcessEvents() {
  while (!events_.empty()) {
    const SDL_Event& event = events_.front();
    events_.pop();

    Action action;

    if (event.type == SDL_MOUSEMOTION) {
      action.type = Action::Type::Hover;
      action.hover.x = event.motion.x;
      action.hover.y = event.motion.y;
    } else if (event.type == SDL_MOUSEBUTTONUP) {
      action.type = Action::Type::Click;
      action.click.x = event.button.x;
      action.click.y = event.button.y;
    } else if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_LEFT) {
        action.type = Action::Type::Move;
        action.move.direction = Action::Move::Direction::Left;
      } else if (event.key.keysym.sym == SDLK_RIGHT) {
        action.type = Action::Type::Move;
        action.move.direction = Action::Move::Direction::Right;
      } else if (event.key.keysym.sym == SDLK_UP) {
        action.type = Action::Type::Move;
        action.move.direction = Action::Move::Direction::Up;
      } else if (event.key.keysym.sym == SDLK_DOWN) {
        action.type = Action::Type::Move;
        action.move.direction = Action::Move::Direction::Down;
      } else if (event.key.keysym.sym == SDLK_ESCAPE) {
        action.type = Action::Type::Deselect;
      } else if (event.key.keysym.sym == SDLK_RETURN) {
        action.type = Action::Type::Select;
      } else {
        continue;
      }
    } else {
      continue;
    }

    uiTree_.root->element->ProcessAction(focusTracker_, action);
  }

  if (!focusTracker_.HasFocusedElement()) {
    interface_.RequestHide();
    FocusRoot();
  }
}

void UiDrawList::CalculateLayout(const glm::vec2 windowSize) {
  uiTree_.root->element->CalculateLayout(
      LayoutCalculator(fontAtlas_, Rectu::FromPoints(0, 0, windowSize)));
}

void UiDrawList::Render(const glm::vec2 windowSize) {
  const Canvas canvas(fontAtlas_, draw_.vertices, text_.vertices);

  uiTree_.root->element->Render(
      UiRenderer(canvas, Rectu::FromRegion(0, 0, windowSize)));

  for (u32 index = 0; index < draw_.vertices.size(); ++index) {
    PositionColorVertex& vertex = draw_.vertices[index];
    vertex.position = NormalizedToNdc(
        glm::vec3(glm::vec2(vertex.position) / windowSize, 0.0f));
    draw_.indices.push_back(index);
  }
  for (u32 index = 0; index < text_.vertices.size(); ++index) {
    TextVertex& vertex = text_.vertices[index];
    vertex.position = NormalizedToNdc(
        glm::vec3(glm::vec2(vertex.position) / windowSize, 0.0f));
    text_.indices.push_back(index);
  }
}

void UiDrawList::FocusRoot() {
  focusTracker_.AcquireFocus(uiTree_.root->element.get());
}
