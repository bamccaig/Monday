#include "Lua_wrapper.h"
#include "Debug.h"
#include <iostream>

std::map<lua_State *, Game *> Lua_wrapper::gamemap;

Lua_wrapper::Lua_wrapper()
	: state(NULL)
{
	// Default constructor
}


Lua_wrapper::~Lua_wrapper()
{
	if (state != NULL)
	{
		Deinit();
	}
}


void Lua_wrapper::Init(Game *game)
{
	state = lua_open();
	luaL_openlibs(state);
	gamemap[state] = game;
}


void Lua_wrapper::Deinit()
{
	lua_close(state);
	state = NULL;
}


void Lua_wrapper::Load_script(std::string filename, bool call)
{
	if (luaL_loadfile(state, filename.c_str()) == 0)
	{
		if (call)
		{
			int errors = lua_pcall(state, 0, 0, 0);
			if (errors)
			{
				std::cout << lua_tostring(state, -1) << std::endl;
			}
		}
	}
	else
	{
		std::cout << filename << " failed to execute" << std::endl;
	}
}


void Lua_wrapper::Call(std::string function, void *caller)
{
	if (function == "")
	{
		return;
	}

	lua_getglobal(state, function.c_str());
	lua_pushlightuserdata(state, caller);
	int errors = lua_pcall(state, 1, 0, 0);

	if (errors)
	{
		std::cout << lua_tostring(state, -1) << std::endl;
	}
}


lua_State *Lua_wrapper::Prepare(std::string function, void *caller)
{
	if (function == "")
	{
		return NULL;
	}

	lua_getglobal(state, function.c_str());
	lua_pushlightuserdata(state, caller);

	return state;
}


void Lua_wrapper::Call(int args)
{
	int errors = lua_pcall(state, args + 1, 0, 0);

	if (errors)
	{
		std::cout << lua_tostring(state, -1) << std::endl;
	}
}


lua_State *Lua_wrapper::Get_state()
{
	return state;
}


Game *Lua_wrapper::Get_game(lua_State *state)
{
	assert(gamemap.find(state) != gamemap.end());
	if (state != NULL)
	{
		return gamemap[state];
	}
	else
	{
		return NULL;
	}
}
