#include "ProjectData.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Scene/Scene.hpp>

namespace GameEngine {
	ProjectData::ProjectData(const std::string& path)
	{
	}

	ProjectData::ProjectData()
	{
	}

	void ProjectData::Deserialize()
	{
	}

	void ProjectData::LoadScene(const std::string& name)
	{
	}
	void ProjectData::SetCurrentScene(const std::string& name)
	{
		if (LoadedScenes.find(name) != LoadedScenes.end())
		{
			Engine::GetInstance()->SetCurrentScene(LoadedScenes[name]);
		}
	}
	int ProjectData::UnloadScene(const std::string& name) {
		return 0;
	}
	std::shared_ptr<Scene> ProjectData::CreateNewScene(const std::string& name)
	{
		auto scene = std::make_shared<Scene>(name);
		LoadedScenes[name] = scene;
		
		return scene;
	}
}
