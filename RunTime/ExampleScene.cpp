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
		GameEngine::Entity ent = CreateEntity("RenderENT1");
		GameEngine::Entity ent2 = CreateEntity("RenderENT2");

		ent2.AddComponent<GameEngine::Renderable>(GameEngine::TextureResourceManager::GetInstance()->GetTexture("Tex1"));
		ent.GetComponent<GameEngine::TransformComponent>().position = glm::vec3(1, 0, 0);
		ent.AddComponent<GameEngine::Renderable>(GameEngine::TextureResourceManager::GetInstance()->GetSubTexture("GroundTile1"));
	}
	void ExampleScene::Update(float Update)
	{
	}
}