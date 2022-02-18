#pragma once

#include <entt/entt.hpp>

namespace GameEngine {
    class NativeScript {
    public:
        ~NativeScript() = default;
        NativeScript(entt::entity entity) : ent(entity) {}

        virtual void Init() {};
        virtual void Update(float delaTime){};
        entt::entity ent;
    };
}