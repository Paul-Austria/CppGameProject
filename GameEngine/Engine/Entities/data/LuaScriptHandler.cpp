#include "LuaScriptHandler.hpp"
#include "DataStructs.hpp"
#include <spdlog/spdlog.h>
namespace GameEngine {
	LuaScriptHandler::LuaScriptHandler()
	{
		lua = sol::state();
		lua.open_libraries(sol::lib::base, sol::lib::package);
	}
	LuaScript LuaScriptHandler::GenerateScript(const std::string& script)
	{
		sol::environment env = sol::environment(lua, sol::create, lua.globals());
		auto fx = this->lua.load_file(script);
		if (!fx.valid()) {
			sol::error err = fx;
			
			spdlog::error("failed to load Script: : {0}",err.what());
			lua.script("function update() end function setup() end",env);
		}
		else
		{
			lua.script_file(script, env);
		}
		LuaScript lscript = LuaScript(script, env, this);
		return lscript;
	}
}