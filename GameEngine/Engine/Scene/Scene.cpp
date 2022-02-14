#include "Scene.hpp"
#include <spdlog/spdlog.h>

#include "Engine/Renderer/ImGUI/imgui/imgui.h"
#include "Engine/Renderer/ImGUI/imgui/imgui_internal.h"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/ImGUI/ImGUIStyling.hpp"


#include <Engine/Entities/BaseComponents.hpp>

#include <Engine/Utils/Profiling/ProfileInstance.hpp>


#include <Engine/Entities/Entity.hpp>
#include <Engine/ResourceManagement/TextureResourceManager.hpp>
#include <Engine/Renderer/Renderable.hpp>
#include <Engine/Core/Window.hpp>

namespace GameEngine {
	Scene::Scene()
	{
		registry = entt::registry();
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = "Test";


		entity.AddComponent<CameraComponent>();
		entity.AddComponent<Renderable>();
		DestroyEntity(entity);


		editorView = EditorView(this);
	}
	Scene::~Scene()
	{
		registry.clear();
	}




	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;


	//	entity.AddComponent<GameRenderable>(GameTextureResourceManager::GetInstance()->GetTexture("Tex2"));


		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		registry.destroy(entity);
	}


	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
	}



	void Scene::BackgroundUpdate(float deltaTime,bool isRunning)
	{

		ProfileInstance::GetInstance()->StartProfileSession("Renderer");


		auto renderer = Renderer::GetInstance();
		
		renderer->BeginRender(editorCam, renderTarget);


		auto renderData = registry.view<Renderable>();
		
		for (auto ent : renderData)
		{
			auto rend = registry.get<Renderable>(ent);
			auto transform = registry.get<TransformComponent>(ent);

			renderer->RenderQuad(rend, transform, editorCam);
		}

		renderer->EndRender(),


		ProfileInstance::GetInstance()->EndProfileSession("Renderer");


	}

	void Scene::EditorUpdate(float deltaTime) {
		
		editorView.EditorUpdate(deltaTime);

	//	ProfileInstance::GetInstance()->PrintDataToTerminal();
	}


	void Scene::UpdateEditorCam(float deltaTime)
	{
		float speed = 0.01;
		if (glfwGetKey(Window::GetInstance()->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
		{
			editorCam.position.y -= speed;
		}
		if (glfwGetKey(Window::GetInstance()->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
		{
			editorCam.position.y += speed;
		}
		if (glfwGetKey(Window::GetInstance()->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
		{
			editorCam.position.x += speed;
		}
		if (glfwGetKey(Window::GetInstance()->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
		{
			editorCam.position.x -= speed;
		}
		if (glfwGetKey(Window::GetInstance()->GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
		{
			editorCam.zoom += 0.1f;
		}

		if (glfwGetKey(Window::GetInstance()->GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
		{
			editorCam.zoom -= 0.1f;
		}
	}


}