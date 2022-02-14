#pragma once
#include <entt/entt.hpp>

namespace GameEngine {
	class Scene;

	class EditorView
	{
	public:
		EditorView(Scene* currentScene);
		EditorView() = default;
		~EditorView() = default;


		void EditorUpdate(float deltaTime);

	private:
		Scene* scene;
		entt::entity currentEntity;
		bool entitySelected = false;
	};
}