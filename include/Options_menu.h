#ifndef Options_menu_h
#define Options_menu_h

#include "Menu.h"
#include "StdTypes.h"

#include "Debug.h"
#include "Game.h"
#include "Main.h"
#include "StdTypes.h"

#include <string>
#include <vector>


class Options_menu: public Menu
{
public:
	Options_menu(Game* game);
	void Render();
	game_event_n Event(ALLEGRO_EVENT event);

private:
	int option;
	std::vector<std::string> optionStrings;
};

#endif
