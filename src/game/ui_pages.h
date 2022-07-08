#ifndef SRC_GAME_UI_PAGES_H_
#define SRC_GAME_UI_PAGES_H_

#define BEGIN_PAGE(NAME) inline constexpr const char* NAME =
#define END_PAGE ;

BEGIN_PAGE(PAUSE_MENU)
#include "ui_pages/pause_menu.xml"
END_PAGE

#endif  // SRC_GAME_UI_PAGES_H_
