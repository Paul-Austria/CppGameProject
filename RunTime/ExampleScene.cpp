#include "ExampleScene.hpp"


#include <Engine/ResourceManagement/TextureResourceManager.hpp>


#include <Engine/Utils/Serialisation/TextureLoader.hpp>
#include <Engine/Entities/Entity.hpp>

#include <Engine/Entities/BaseComponents.hpp>
#include <Engine/Renderer/Renderable.hpp>
namespace TestNamespace {
	void ExampleScene::Init()
	{

		GameEngine::TextureLoader::LoadTexturesByJson("Resources/textureLoader.json");
		GameEngine::Entity ent = CreateEntity("TestEntity");
		CreateEntity("TestEntity1");
		CreateEntity("TestEntity4");
		CreateEntity("TestEntity5");
		CreateEntity("TestEntity6");
		CreateEntity("TestEntity7");
		CreateEntity("TestEntity8");

		ent.AddComponent<GameEngine::Renderable>(GameEngine::TextureResourceManager::GetInstance()->GetTexture("Tex2"));
	}
	void ExampleScene::Update(float Update)
	{
	}
}