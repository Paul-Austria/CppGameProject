#include "Scene.hpp"
#include <spdlog/spdlog.h>

#include "Engine/Renderer/ImGUI/imgui/imgui.h"
#include "Engine/Renderer/ImGUI/imgui/imgui_internal.h"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/ImGUI/ImGUIStyling.hpp"


#include <Engine/Entities/BaseComponents.hpp>
#include <Engine/Entities/data/LuaScript.hpp>
#include <Engine/Utils/Profiling/ProfileInstance.hpp>


#include <Engine/Entities/Entity.hpp>
#include <Engine/ResourceManagement/TextureResourceManager.hpp>
#include <Engine/Renderer/Renderable.hpp>
#include <Engine/Core/Window.hpp>


#include <Engine/Entities/data/LuaScriptHandler.hpp>
#include <algorithm>


namespace GameEngine {
	Scene::Scene(const std::string& sceneName)
	{
		this->sceneName = sceneName;
		registry = entt::registry();

#ifdef DEBUG
		saveRegistry = entt::registry();
#endif // DEBUG


		luaHandler = LuaScriptHandler();

		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = "Test";

		entity.AddComponent<CameraComponent>();
		entity.AddComponent<Renderable>();
		entity.AddComponent<NativeScriptHolder>(std::make_shared<NativeScript>(entity.GetEntity()));
		auto script = luaHandler.GenerateScript("", entity);
		entity.AddComponent<LuaScript>(script);

		DestroyEntity(entity);





		editorView = EditorView(this);
	}
	Scene::~Scene()
	{
		registry.clear();
	}

	Scene::Scene(const std::string& Path, bool usePath)
	{
	}



	void Scene::ChangeStatus(SceneStatus cStatus)
	{
		if (cStatus == Running && status == Stopped) {
			camera = GameCamera;
			CopyRegsitry(registry, saveRegistry);


			auto scripts = registry.view<LuaScript>();

			for (auto ent : scripts)
			{
				LuaScript& script = registry.get<LuaScript>(ent);
				script.Reset(Entity(ent, this));
			}
			
		}
		else if (status == Running && cStatus == Stopped)
		{
			camera = EditorCamera;
			CopyRegsitry(saveRegistry, registry);
		}
		else if (status == Running && cStatus == Paused)
		{
			camera = EditorCamera;
		}
		this->status = cStatus;
		
	}

	void Scene::ChangeActiveCamera(ActiveCamera cam)
	{
		camera = cam;
	}

	Entity Scene::GetEntityByTag(std::string name)
	{
		auto tags = registry.view<TagComponent>();
		for (auto ent : tags)
		{
			auto tag = registry.get<TagComponent>(ent).Tag;
			tag.erase(std::remove(tag.begin(), tag.end(), '\0'),tag.end());
			if (tag == name)
			{
				return Entity{ent, this};
			}
		}
		return Entity();
	}






	void Scene::BackgroundUpdate(float deltaTime,bool isRunning)
	{
		ProfileInstance::GetInstance()->StartProfileSession("SceneBackground_"+this->sceneName);

		CameraComponent activeCam = editorCam;
		if (camera == GameCamera)
		{
			auto cameraData = registry.view<CameraComponent>();
			for (auto cam : cameraData)
			{
				auto camera = registry.get<CameraComponent>(cam);
				if (camera.active)
				{
					activeCam = camera;
					activeCam.position = registry.get<TransformComponent>(cam).position;
					break;
				}

			}
		}

		if (this->status == Running)
		{
			ProfileInstance::GetInstance()->StartProfileSession("NativeScripts");

			auto nativeScripts = registry.view<NativeScriptHolder>();

			for (auto ent : nativeScripts)
			{
				registry.get<NativeScriptHolder>(ent).nativeScript->Update(deltaTime);
			}

			ProfileInstance::GetInstance()->EndProfileSession("NativeScripts");
			ProfileInstance::GetInstance()->StartProfileSession("LuaScripts");


			auto scripts = registry.view<LuaScript>();

			for (auto ent : scripts)
			{
				LuaScript& script = registry.get<LuaScript>(ent);
				script.RunUpdate();
			}

			ProfileInstance::GetInstance()->EndProfileSession("LuaScripts");
		}
		


		ProfileInstance::GetInstance()->StartProfileSession("Renderer");

		ProfileInstance::GetInstance()->StartProfileSession("Renderer");

		auto renderer = Renderer::GetInstance();
		
		renderer->BeginRender(activeCam, renderTarget);

		auto renderData = registry.view<Renderable>();
		
		for (auto ent : renderData)
		{
			auto rend = registry.get<Renderable>(ent);
			auto transform = registry.get<TransformComponent>(ent);

			renderer->RenderQuad(rend, transform, activeCam);
		}

		renderer->EndRender(),


		ProfileInstance::GetInstance()->EndProfileSession("Renderer");

		ProfileInstance::GetInstance()->EndProfileSession("SceneBackground_" + this->sceneName);

	}

	void Scene::EditorUpdate(float deltaTime) {
		
		editorView.EditorUpdate(deltaTime);

		
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
			editorCam.zoom += 0.01f;
		}

		if (glfwGetKey(Window::GetInstance()->GetWindow(), GLFW_KEY_E) == GLFW_PRESS)
		{
			editorCam.zoom -= 0.01f;
		}
	}



#pragma region  ENTT_UTILS




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

	void Scene::CopyRegsitry(entt::registry& source, entt::registry& target)
	{
		target.clear();
		target = entt::registry();

		auto v = source.view<TagComponent>();
		for (auto en : v)
		{
			auto enti = target.create();
			auto tagC = source.get<TagComponent>(en);
			auto tran = source.get<TransformComponent>(en);
			target.emplace<TagComponent>(enti,tagC.Tag);
			target.emplace<TransformComponent>(enti, tran.position, tran.rotation, tran.scale);
			if (source.all_of<Renderable>(en))
			{
				auto rendercom = source.get<Renderable>(en);
				target.emplace<Renderable>(enti, rendercom);
			}
			if (source.all_of<CameraComponent>(en))
			{
				auto rendercom = source.get<CameraComponent>(en);
				target.emplace<CameraComponent>(enti, rendercom);
			}
			if (source.all_of<LuaScript>(en))
			{
				auto rendercom = source.get<LuaScript>(en);
				target.emplace<LuaScript>(enti, rendercom);
			}
		}
		
	}




	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
	}

#pragma endregion


}