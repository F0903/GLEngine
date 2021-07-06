module;
#include <lua.hpp>
export module ScriptEngine;

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
	lua_State* lua;

	private:
	void Initialize()
	{
		lua = lua_open();
		luaL_openlibs(lua);
	}

	void Close()
	{
		lua_close(lua);
	}

	public:
	void Run(const char* file)
	{
		if (luaL_loadfile(lua, file) || lua_pcall(lua, 0, 0, 0))
			throw "Could not file";
	}
};