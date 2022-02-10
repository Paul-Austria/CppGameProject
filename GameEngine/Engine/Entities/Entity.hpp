#pragma once

#include <entt/entt.hpp>
#include "Engine/Scene/Scene.hpp"

namespace GameEngine {
	class Entity
	{
	public:
        Entity() = default;
        Entity(entt::entity handle, Scene * scene) : m_EntityHandle(handle), m_Scene(scene)
        {}
        ~Entity() {}

#pragma region entt
        template <typename T, typename... Args>
        T& AddComponent(Args &&...args)
        {
            T& component = m_Scene->registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template <typename T>
        T& GetComponent()
        {
            return m_Scene->registry.get<T>(m_EntityHandle);
        }

        template <typename T>
        bool HasComponent()
        {
            return m_Scene->registry.get<T>(m_EntityHandle) != nullptr;
        }

        template <typename T>
        void RemoveComponent()
        {
            m_Scene->registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        bool operator==(const Entity & other) const
        {
            return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
        }

        bool operator!=(const Entity & other) const
        {
            return !(*this == other);
        }


	private:
		
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}