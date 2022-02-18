#pragma once
#include <memory>
#include <string>
#include <Engine/Scene/ProjectData.hpp>
#include <memory>

namespace GameEngine {
	class Scene;
	class EditorView;
	class Engine
	{
	public:
		Engine();
		~Engine();

		void Init(int widht, int height, const std::string& title,const std::string &ProjectPath = "", bool isFullScreen = false);
		void Loop();
		
		void SetCurrentScene(std::shared_ptr<Scene> Scene);
		
		static Engine* GetInstance();

		void Reset();


		void LoadProject(const std::string& path);

	private:

		friend class EditorView;

		void DevelopUpdate(float deltaTime);
		void SceneUpdate(float deltaTime);

		static Engine* instance;
		

		ProjectData currentProject;

		std::shared_ptr<Scene> currentScene;
		float LastTime = 0;
		bool isRunning = true;

	};
}