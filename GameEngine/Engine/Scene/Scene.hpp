#pragma once

#include <entt/entt.hpp>
#include <string>
#include <Engine/Entities/BaseComponents.hpp>

namespace GameEngine {
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();


		virtual void Init() {};
		virtual void Update(float Update) {};


		Entity CreateEntity(std::string name);



		void DestroyEntity(Entity ent);

		template <typename T>
		void OnComponentAdded(Entity entity, T& component);



	private:
		friend class Engine;
		friend class Entity;


		CameraComponent editorCam;

		void BackgroundUpdate(float deltaTime, bool isRunning);
		void EditorUpdate(float deltaTime);
		entt::registry registry;


		bool useEditorCam = true; 
	};
}