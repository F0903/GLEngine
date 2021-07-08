module;
#include <format>
#include <vector>
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

export int GetNumber(LuaState* lua, int argIndex)
{
	return luaL_checknumber(lua, argIndex);
}

export void PushBoolean(LuaState* lua, bool value)
{
	lua_pushboolean(lua, value);
}

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
	std::vector<void(*)()> beforeUpdate;
	std::vector<void(*)()> afterUpdate;

	private:
	void Initialize()
	{
		lua = lua_open();
		luaL_openlibs(lua);
	}

	void PushUpdateFunc() const
	{
		lua_getglobal(lua, "Update");
		if (!lua_isfunction(lua, 1))
		{
			throw "Update was not a function.";
		}
	}

	void Close() const
	{
		lua_close(lua);
	}

	public:
	void Run(const char* file) const
	{
		luaL_dofile(lua, file);
		PushUpdateFunc();
	}

	void Update() const
	{
		for (const auto f : beforeUpdate)
			f();

		if (lua_pcall(lua, 0, 0, 0) != 0) //TODO: Will except if called more than once.
		{
			throw "Error running update";
		}

		for (const auto f : afterUpdate)
			f();
	}

	void ExposeFn(LuaFn fn) const
	{
		lua_pushcfunction(lua, fn.func);
		lua_setglobal(lua, fn.name);
	}

	void OnBeforeUpdate(void(*fn)())
	{
		beforeUpdate.push_back(fn);
	}

	void OnAfterUpdate(void(*fn)())
	{
		afterUpdate.push_back(fn);
	}
};