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

export struct PctOrNum
{
	const float value;
	const bool isPct;
};

int pow(int num, int pow)
{
	int total = num;
	for (size_t i = 0; i < pow; i++)
		total *= total;
	return total;
}

export const char* GetString(LuaState* lua, int argIndex)
{
	return luaL_checkstring(lua, argIndex);
}

export float GetPercentage(LuaState* lua, int argIndex)
{
	const char* str = GetString(lua, argIndex);
	int len = 0;
	int total = 0;
	while (*str)
	{
		++len;
		++str;
	}
	if (*(str - 1) != '%')
		return -1;
	for (size_t i = 0; i < len - 1; i++)
	{
		const char ch = *(str - i - 2);
		const int num = ch - 48;
		const int step = pow(10, i);
		total += num * step;
	}
	return total;
}

export int GetNumber(LuaState* lua, int argIndex)
{
	return luaL_checknumber(lua, argIndex);
}

export PctOrNum GetPctOrNum(LuaState* lua, int argIndex)
{
	float val;
	if ((val = GetPercentage(lua, argIndex)) > 0)
		return { val, true };
	return { (float)GetNumber(lua, argIndex), false };
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

	//TODO: Find a way to cache textures and set them without loading them from disk constantly.
	// Like some kind of texture object or something.
	public:
	void Run(const char* file) const
	{
		luaL_dofile(lua, file);
	}

	void Update() const
	{
		PushUpdateFunc();
		for (const auto f : beforeUpdate)
			f();

		if (lua_pcall(lua, 0, 0, 0) != 0)
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