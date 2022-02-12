#include "Entity.hpp"

#include <Engine/Scene/Scene.hpp>

namespace GameEngine {
 
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

    
}