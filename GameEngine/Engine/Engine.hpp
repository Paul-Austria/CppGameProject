#pragma once
#include <memory>
#include <string>
namespace GameEngine {
	class Scene;

	class Engine
	{
	public:
		Engine();
		~Engine();

		void Init(int widht, int height, const std::string& title, bool isFullScreen = false);
		void Loop();
		
		void SetCurrentScene(Scene* Scene);
		
		static Engine* GetInstance();

		void Reset();

	private:
		void DevelopUpdate(float deltaTime);
		void SceneUpdate(float deltaTime);

		static Engine* instance;
		


		Scene* currentScene;
		float LastTime = 0;
		bool isRunning = true;

	};
}