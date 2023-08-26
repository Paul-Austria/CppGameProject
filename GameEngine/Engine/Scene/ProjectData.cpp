#include "ProjectData.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Scene/Scene.hpp>
#include "Engine/Utils/Serialisation/ProjectSerialisation.hpp"
namespace GameEngine {
	ProjectData::ProjectData(const std::string& path)
	{
		this->SetPath(path);
	}

	ProjectData::ProjectData()
	{
	}

	void ProjectData::Deserialize()
	{
	}

	bool ProjectData::LoadScene(const std::string& name)
	{
		//Check if already loaded 
		if (LoadedScenes.find(name) != LoadedScenes.end()) {
			return true;
		}
		std::shared_ptr<Scene> ret =  ProjectSerialisation::LoadScene(this->Path + "/" + name + ".scjson");
		if (ret != nullptr){
			LoadedScenes[name] = ret;
			return true;
		}
		return false;
	}
	bool ProjectData::SetCurrentScene(const std::string& name, bool isActive)
	{
		if (LoadedScenes.find(name) != LoadedScenes.end())
		{
			if (isActive) {
				LoadedScenes[name]->ChangeStatus(Running);
			}
			Engine::GetInstance()->PrepareSceneChange(name);
//			Engine::GetInstance()->GetCurrentScene()->ChangeStatus(Stopped);
//			LoadedScenes[name]->ChangeStatus(Running);
//			Engine::GetInstance()->SetCurrentScene(LoadedScenes[name]);
			return true;
		}
		return false;
	}
	bool ProjectData::UnloadScene(const std::string& name) {
		return true;
	}
	std::shared_ptr<Scene> ProjectData::CreateNewScene(const std::string& name)
	{
		auto scene = std::make_shared<Scene>(name);
		LoadedScenes[name] = scene;
		
		return scene;
	}
}
