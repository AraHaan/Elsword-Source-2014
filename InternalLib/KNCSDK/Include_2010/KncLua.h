#pragma once

//extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
//}

#define DeclLuaValue void to_lua(lua_State *L) { lua_tinker::push_::invoke( L, this); }

//{{ 2009. 7. 24  ������	lua�Լ� ��ũ��
#define LUA_DOFILE luaL_dofile
//}}