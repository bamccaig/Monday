#ifndef Gui_h
#define Gui_h


#include "main.h"
#include "Game.h"
#include "Game_events.h"

class Menu;


class Gui
{
	public:
		Gui(Game *game);
		ALLEGRO_FONT *Get_font();
		void Set_font(ALLEGRO_FONT *font);

		ALLEGRO_FONT *font;
};


class Button
{
	public:
		Button(Menu *_menu);
		Button(Gui *_gui);
		Button(Gui *_gui, int cx, int cy, int w, int h, char *string, char *tag);
		~Button();

		void Draw(int x, int y, bool selected);
		void Draw_Centre(int x, int y, bool selected);	

	protected:
		game_event_n action; 

	private:
		Gui *gui;
		Menu *menu;
		int  x, y, width, height;
		char *string;
		char *tag;
};


#endif  //  Gui_h
