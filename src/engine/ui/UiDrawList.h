#ifndef SRC_ENGINE_UI_UIDRAWLIST_H_
#define SRC_ENGINE_UI_UIDRAWLIST_H_

#include <queue>

#include "DrawList.h"
#include "FocusTracker.h"
#include "UiElement.h"
#include "UiTree.h"
#include "core/text/FontAtlas.h"
#include "engine/behaviour/Updateable.h"
#include "engine/property/type/GraphicalInterface.h"
#include "renderer/vertices/PositionColorVertex.h"
#include "renderer/vertices/TextVertex.h"
#include "system/windowing/WindowEventConsumer.h"

class UiDrawList : public WindowEventConsumer,
                   public DrawList,
                   public Updateable {
 public:
  UiDrawList(const FontAtlas& fontAtlas, GraphicalInterface& interface,
             UiTree uiTree, glm::vec2 windowSize);

 private:
  void Consume(const SDL_Event& event) override;
  DrawList::DrawData Generate(glm::vec2 windowSize) override;
  void UpdateModel(const UpdateContext& context) override;

  void ProcessEvents();
  void CalculateLayout(glm::vec2 windowSize);
  void Render(glm::vec2 windowSize);

  void FocusRoot();

 private:
  const FontAtlas& fontAtlas_;
  GraphicalInterface& interface_;
  UiTree uiTree_;
  FocusTracker focusTracker_;
  std::queue<SDL_Event> events_;
  DrawVertices<PositionColorVertex> draw_;
  DrawVertices<TextVertex> text_;
};

#endif  // SRC_ENGINE_UI_UIDRAWLIST_H_
