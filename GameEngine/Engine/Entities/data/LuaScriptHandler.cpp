#include "LuaScriptHandler.hpp"
#include "DataStructs.hpp"
#include <spdlog/spdlog.h>
#include <Engine/Engine.hpp>
#include <Engine/Scene/ProjectData.hpp>
namespace GameEngine {
	LuaScriptHandler::LuaScriptHandler()
	{
		lua = sol::state();
		lua.open_libraries(sol::lib::base, sol::lib::package);
	}
	LuaScript LuaScriptHandler::GenerateScript(const std::string& script)
	{
		
		sol::environment env = sol::environment(lua, sol::create, lua.globals());

		std::string fullPath = Engine::GetInstance()->GetCurrentProject().GetPath() + "/" + script;
		auto fx = this->lua.load_file(fullPath);
		if (!fx.valid()) {
			sol::error err = fx;
			
			spdlog::error("failed to load Script: : {0}",err.what());
			lua.script("function update() end function setup() end",env);
		}
		else
		{
			lua.script_file(fullPath, env);
		}
		LuaScript lscript = LuaScript(script, env, this);
		return lscript;
	}
}