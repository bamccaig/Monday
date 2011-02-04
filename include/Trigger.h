#ifndef Trigger_h
#define Trigger_h

#include <allegro5/allegro5.h>
extern "C"
{
#include "allegro5/a5_ttf.h"
}


typedef enum {
TRIGGER_TYPE_NONE, 
TRIGGER_TYPE_PROXIMITY,

TRIGGER_TYPE_USER
} trigger_t;


class Trigger
{
private:
	trigger_t trigger_type;
	Action * action;
};

class Trigger_proximity : public Trigger() 
{
private:
	Item * item1;
	Item * item2;
	float distance;
};





#endif
