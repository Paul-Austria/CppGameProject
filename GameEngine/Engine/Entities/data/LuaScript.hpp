#pragma once
#include <sol/sol.hpp>
#include <Engine/Entities/Entity.hpp>
#include "LuaScriptHandler.hpp"
#include <string>
#include <Engine/Engine.hpp>
namespace GameEngine {

    struct LuaScript {
        std::string scriptPath;
        sol::environment environment;
        LuaScriptHandler* parentHandler;
        Entity entity;
        bool alreadyRun = false;

        LuaScript(std::string path, sol::environment env, LuaScriptHandler* handler, Entity entity) {
            this->scriptPath = path;
            this->environment = env;
            this->parentHandler = handler;
            env["self"] = entity;
            env["Engine"] = Engine::GetInstance();
        }

        LuaScript() {

        };


        template<typename T> T GetVar(const std::string& name)
        {
            return environment[name];
        }


        void Reset(Entity ent) {

            LuaScript s = parentHandler->GenerateScript(scriptPath, ent);
            this->environment = s.environment;
            this->entity = s.entity;
            alreadyRun = false;
        }

        void RunUpdate(float deltaTime) {
            if (!alreadyRun)
            {
                alreadyRun = true;
                try
                {
                    sol::function setup = this->environment["setup"];
                    setup();
                }
                catch (const std::exception& exc)
                {
                    spdlog::error("Lua script error: {0}", exc.what());
                }

            }
            try
            {
                sol::function update = this->environment["update"];
                update(deltaTime);
            }
            catch (const std::exception& exc)
            {
                spdlog::error("Lua script error: {0}", exc.what());
            }
        }
    };
}