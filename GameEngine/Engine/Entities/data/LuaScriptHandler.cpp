#include "LuaScriptHandler.hpp"
#include "DataStructs.hpp"
#include <spdlog/spdlog.h>
#include <Engine/Engine.hpp>
#include <Engine/Scene/ProjectData.hpp>
#include <Engine/Entities/data/LuaScript.hpp>
#include <glm/glm.hpp>
#include <Engine/Scene/Scene.hpp>
#include <Engine/Entities/Entity.hpp>
#include <Engine/Entities/BaseComponents.hpp>

#include <Engine/Entities/Entity.hpp>

namespace GameEngine {
	LuaScriptHandler::LuaScriptHandler()
	{
		lua = sol::state();
		lua.open_libraries(sol::lib::base, sol::lib::package);



		CreateGlMUserTypes();

		auto eng = lua["Engine"].get_or_create<sol::table>();
		auto entit = lua["Engine"]["Entities"].get_or_create<sol::table>();

		entit.new_usertype<TagComponent>("TagComponent",
			"Tag", &TagComponent::Tag
			);
		entit.new_usertype<TransformComponent>("TransformComponent",
			"position",&TransformComponent::position,
			"rotation",&TransformComponent::rotation,
			"scale",&TransformComponent::scale
			);
		entit.new_usertype<Entity>("Entity",
			"GetTagComponent", &Entity::GetComponent<TagComponent>,
			"GetTransformComponent", &Entity::GetComponent<TransformComponent>
			);
		eng.new_usertype<Scene>("Scene",
			"GetSceneName",&Scene::GetSceneName,
			"CreateEntity",&Scene::CreateEntity,
			"DestroyEntity",&Scene::DestroyEntity
			);
		eng.new_usertype<Engine>("Engine",
			"GetInstance", &Engine::GetInstance,
			"GetCurrentScene", &Engine::GetCurrentScene
			);
	}
	LuaScript LuaScriptHandler::GenerateScript(const std::string& script, Entity entity)
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
		LuaScript lscript = LuaScript(script, env, this, entity);
		return lscript;
	}

#pragma region UserTypesGlM
	void LuaScriptHandler::CreateGlMUserTypes()
	{
		auto mult_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 * v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 * f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f * v1; }
		);

		auto add_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 + v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 + f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f + v1; }
		);

		auto sub_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 - v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 - f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f - v1; }
		);

		auto devision_overload = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 { return v1 / v2; },
			[](const glm::vec3& v1, float f) -> glm::vec3 { return v1 / f; },
			[](float f, const glm::vec3& v1) -> glm::vec3 { return f / v1; }
		);

		
		
		auto compare_overload = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) -> bool { return v1 == v2; }
		);

		auto glm_vec3 = lua["glm"].get_or_create<sol::table>();
		glm_vec3.new_usertype<glm::vec3>("vec3",
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			sol::meta_function::multiplication, mult_overloads,
			sol::meta_function::addition, add_overloads,
			sol::meta_function::subtraction, sub_overloads,
			sol::meta_function::division, devision_overload,
			sol::meta_function::equal_to, compare_overload
			);
//============================================================vec2

		auto mult_overloads2 = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) -> glm::vec2 { return v1 * v2; },
			[](const glm::vec2& v1, float f) -> glm::vec2 { return v1 * f; },
			[](float f, const glm::vec2& v1) -> glm::vec2 { return f * v1; }
		);

		auto add_overloads2 = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) -> glm::vec2 { return v1 + v2; },
			[](const glm::vec2& v1, float f) -> glm::vec2 { return v1 + f; },
			[](float f, const glm::vec2& v1) -> glm::vec2 { return f + v1; }
		);

		auto sub_overloads2 = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) -> glm::vec2 { return v1 - v2; },
			[](const glm::vec2& v1, float f) -> glm::vec2 { return v1 - f; },
			[](float f, const glm::vec3& v1) -> glm::vec2 { return f - v1; }
		);

		auto devision_overload2 = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) -> glm::vec2 { return v1 / v2; },
			[](const glm::vec2& v1, float f) -> glm::vec2 { return v1 / f; },
			[](float f, const glm::vec2& v1) -> glm::vec2 { return f / v1; }
		);



		auto compare_overload2 = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) -> bool { return v1 == v2; }
		);

		glm_vec3.new_usertype<glm::vec2>("vec2",
			"x", &glm::vec2::x,
			"y", &glm::vec2::y,
			sol::meta_function::multiplication, mult_overloads2,
			sol::meta_function::addition, add_overloads2,
			sol::meta_function::division, devision_overload2,
			sol::meta_function::subtraction, sub_overloads2,
			sol::meta_function::equal_to, compare_overload2
			);

//================================================vec4



		auto mult_overloads4 = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) -> glm::vec4 { return v1 * v2; },
			[](const glm::vec4& v1, float f) -> glm::vec4 { return v1 * f; },
			[](float f, const glm::vec4& v1) -> glm::vec4 { return f * v1; }
		);

		auto add_overloads4 = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) -> glm::vec4 { return v1 + v2; },
			[](const glm::vec4& v1, float f) -> glm::vec4 { return v1 + f; },
			[](float f, const glm::vec4& v1) -> glm::vec4 { return f + v1; }
		);

		auto sub_overloads4 = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) -> glm::vec4 { return v1 - v2; },
			[](const glm::vec4& v1, float f) -> glm::vec4 { return v1 - f; },
			[](float f, const glm::vec4& v1) -> glm::vec4 { return f - v1; }
		);

		auto devision_overload4 = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) -> glm::vec4 { return v1 / v2; },
			[](const glm::vec4& v1, float f) -> glm::vec4 { return v1 / f; },
			[](float f, const glm::vec4& v1) -> glm::vec4 { return f / v1; }
		);



		auto compare_overload4 = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) -> bool { return v1 == v2; }
		);

		glm_vec3.new_usertype<glm::vec4>("vec4",
			"x", &glm::vec4::x,
			"y", &glm::vec4::y,
			"z", &glm::vec4::z,
			"a", &glm::vec4::a,
			sol::meta_function::multiplication, mult_overloads4,
			sol::meta_function::addition, add_overloads4,
			sol::meta_function::division, devision_overload4,
			sol::meta_function::subtraction, sub_overloads4,
			sol::meta_function::equal_to, compare_overload4
			);

	}
#pragma endregion
}