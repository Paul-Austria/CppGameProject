#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <Engine/sol/assert.hpp>
#include <string>

namespace GameEngine {

	struct LuaScript;
	
	class LuaScriptHandler
	{
	public:
		LuaScriptHandler();

		LuaScript GenerateScript(const std::string& script);
		
		sol::state lua;
	private:
		void CreateGlMUserTypes();

	};

	
}