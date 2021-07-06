module;
#include "lua/lua.hpp"
export module ScriptEngine;

export using LuaState = lua_State;
export using LuaCFunc = lua_CFunction;
export using LuaLibArray = luaL_Reg;

export struct LuaFn
{
	const char* name;
	LuaCFunc func;
};

export struct LuaLib
{
	const char* name;
	LuaLibArray lib;
};

export class ScriptEngine
{
	public:
	ScriptEngine()
	{
		Initialize();
	}

	~ScriptEngine()
	{
		Close();
	}

	private:
	LuaState* lua;

	private:
	void Initialize()
	{
		lua = lua_open();
		luaL_openlibs(lua);
	}

	void Close() const
	{
		lua_close(lua);
	}

	public:
	void Run(const char* file) const
	{
		if (luaL_loadfile(lua, file) || lua_pcall(lua, 0, 0, 0))
			throw "Could not file";
	}

	void ExposeFn(LuaFn fn) const
	{
		lua_pushcfunction(lua, fn.func);
		lua_setglobal(lua, fn.name);
	}
};