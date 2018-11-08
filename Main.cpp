#include "Lua/lua.hpp"
#include <iostream>
#include <stdio.h>

#ifndef _WIN32
#define VITA
#include <psp2/kernel/processmgr.h>
#include "debugScreen.h"
#define printf psvDebugScreenPrintf
#endif

#ifdef _WIN32	
const char * path = R"(C:\Users\noela\AppData\Roaming\Vita3K\Vita3K\ux0\data\Test.lua)";
#else	
const char * path = R"(ux0:data\Test.lua)";
#endif

void DrawRect(int x, int y, int w, int h)
{
	printf("Rectangle : %i %i %i %i\n", x, y, w, h);
}

int MagicDrawRect(lua_State * state)
{
	int x = static_cast<int>(luaL_checkinteger(state, 1));
	int y = static_cast<int>(luaL_checkinteger(state, 2));
	int w = static_cast<int>(luaL_checkinteger(state, 3));
	int h = static_cast<int>(luaL_checkinteger(state, 4));

	DrawRect(x, y, w, h);

	lua_pushinteger(state, 5);
	return 1;
}

int MagicPrintNumber(lua_State * state)
{
	printf("%f\n",luaL_checknumber(state, 1));
	return 0;
}

int MagicPrint(lua_State * state)
{
	printf("%s\n", lua_tostring(state, 1));
	return 0;
}

int main(int argc, char * argv[])
{
#ifdef VITA
	psvDebugScreenInit();
#endif
	lua_State * state = luaL_newstate();
	luaL_openlibs(state);
	
	lua_pushcfunction(state, MagicDrawRect);
	lua_setglobal(state, "DrawRect");

	lua_pushcfunction(state, MagicPrintNumber);
	lua_setglobal(state, "PrintFloat");

	lua_pushcfunction(state, MagicPrint);
	lua_setglobal(state, "PrintString");
	
	auto status = luaL_loadfile(state, path);

	if (status != 0)
	{
		printf("Error loading %d", lua_tostring(state, -1));
		return status;
	}
	
	auto result = lua_pcall(state, 0, LUA_MULTRET, 0);
	if (result != 0)
	{
		printf("Error executing %d", lua_tostring(state, -1));
		return status;
	}
	lua_close(state);
	std::cin.get();

	#ifdef VITA
	sceKernelDelayThread(20*1000*1000);
	sceKernelExitProcess(0);
	#endif
	return 0;
}