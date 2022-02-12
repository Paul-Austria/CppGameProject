#pragma once

#include <Engine/Scene/Scene.hpp>

#include <Engine/ResourceManagement/TextureResourceManager.hpp>


#include <Engine/Utils/Serialisation/TextureLoader.hpp>
#include <Engine/Entities/Entity.hpp>




namespace TestNamespace {
	class ExampleScene : public GameEngine::Scene
	{
	public:
		void Init();
		void Update(float Update);

	private:

	};
}