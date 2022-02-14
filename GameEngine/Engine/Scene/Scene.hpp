#pragma once

#include <string>
#include <entt/entt.hpp>
#include <Engine/Entities/data/DataStructs.hpp>
#include <Engine/Entities/BaseComponents.hpp>
#include <Engine/Editor/EditorView.hpp>

namespace GameEngine {

	class Entity;
	class Scene
	{
	public:
		entt::registry registry;
		Scene();
		~Scene();


		virtual void Init() = 0;
		virtual void Update(float Update) = 0;

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);


		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		friend class Engine;
		friend class Entity;
		friend class EditorView;


		Texture renderTarget;
		EditorView editorView;

		void BackgroundUpdate(float deltaTime, bool isRunning);
		void EditorUpdate(float deltaTime);
		void UpdateEditorCam(float deltaTime);

		bool useEditorCam = true; 
		CameraComponent editorCam;
	};
}