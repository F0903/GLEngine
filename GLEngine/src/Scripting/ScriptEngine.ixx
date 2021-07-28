module;
#include <format>
#include <vector>
#include "lua/lua.hpp"
export module ScriptEngine;
import StringExpression;

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

export const char* GetString(LuaState* lua, int argIndex)
{
	return luaL_checkstring(lua, argIndex);
}

export float GetPercentage(LuaState* lua, int argIndex)
{
	const char* str = GetString(lua, argIndex);
	const auto expr = StringExpression(str);
	return expr.GetNumericValue();
}

export int GetInteger(LuaState* lua, int argIndex)
{
	return luaL_checknumber(lua, argIndex);
}

export PctOrNum GetPctOrNum(LuaState* lua, int argIndex)
{
	float val;
	if ((val = GetPercentage(lua, argIndex)) > 0)
		return { val, true };
	return { (float)GetInteger(lua, argIndex), false };
}

export void PushBoolean(LuaState* lua, bool value)
{
	lua_pushboolean(lua, value);
}

export void PushInteger(LuaState* lua, int value)
{
	lua_pushinteger(lua, value);
}

export struct ScriptEngineException : std::exception
{
	ScriptEngineException(const char* msg) : msg(msg)
	{}

	const char* msg;

	const char* what() const override
	{
		return msg;
	}
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
	}

	void Update() const
	{
		PushUpdateFunc();
		for (const auto f : beforeUpdate)
			f();

		if (lua_pcall(lua, 0, 0, 0) != 0)
		{
			throw "Error running update. Please check your script.";
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