#pragma once

#include <string>
#include <entt/entt.hpp>
#include <Engine/Entities/data/DataStructs.hpp>
#include <Engine/Entities/BaseComponents.hpp>
#include <Engine/Editor/EditorView.hpp>

#include <Engine/Entities/data/LuaScriptHandler.hpp>
namespace GameEngine {
	class Entity;
	enum  SceneStatus {
		Stopped,
		Running,
		Paused
	};
	enum  ActiveCamera {
		EditorCamera,
		GameCamera
	};

	class Scene
	{
	public:
		Scene(const std::string& sceneName = "New_Scene");
		~Scene();
		Scene(const std::string& Path,bool usePath);


		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);


		template<typename T>
		void OnComponentAdded(Entity entity, T& component);



		static void CopyRegsitry(entt::registry& source, entt::registry& target);


		void ChangeStatus(SceneStatus status);

		void ChangeActiveCamera(ActiveCamera cam);


		std::string GetSceneName() { return sceneName; }

	private:
		friend class Engine;
		friend class Entity;
		friend class EditorView;
		friend class ProjectSerialisation;
		friend class ContentBrowser;

		Texture renderTarget;
		EditorView editorView;

		void BackgroundUpdate(float deltaTime, bool isRunning);
		void EditorUpdate(float deltaTime);
		void UpdateEditorCam(float deltaTime);

		bool useEditorCam = true; 
		CameraComponent editorCam;
		SceneStatus status = Stopped;
		ActiveCamera camera = EditorCamera;
		
		LuaScriptHandler luaHandler;

		entt::registry registry;

		std::string sceneName;

#ifdef DEBUG
		entt::registry saveRegistry;
#endif // DEBUG

	};
}