#pragma once
#include <sol/sol.hpp>
#include <Engine/Entities/Entity.hpp>
#include "LuaScriptHandler.hpp"
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
        }

        LuaScript() {

        };


        void Reset(Entity ent) {

            LuaScript s = parentHandler->GenerateScript(scriptPath, ent);
            this->environment = s.environment;
            this->entity = s.entity;
            alreadyRun = false;
        }

        void RunUpdate() {
            if (!alreadyRun)
            {
                alreadyRun = true;
                this->parentHandler->lua.script("setup()", this->environment);

            }
            this->parentHandler->lua.script("update()", this->environment);
        }
    };
}