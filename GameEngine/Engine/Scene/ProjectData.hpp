#pragma once

#include <map>
#include <string>
#include <memory>
namespace GameEngine {	
	class Scene;

	class ProjectData
	{
	public:
		ProjectData(const std::string& path);
		ProjectData();
		~ProjectData() = default;


		void Deserialize();


		void SetName(const std::string& Name) { this->Name = Name; }
		std::string GetName() { return Name; }

		std::string GetPath() { return Path; }
		void LoadScene(const std::string& name);

		void SetCurrentScene(const std::string& name);

		std::shared_ptr<Scene> CreateNewScene(const std::string& name);
	private:

		friend class Engine;
		friend class ProjectSerialisation;

		std::string Path = "D:/PR/GameEngineTesting/TestProject";
		std::string Name = "New Project";
		//Name, Path
		std::map<std::string, std::string> ResourceIncludes;
		//Name, Path
		std::map<std::string, std::string> Scenes;
		std::map<std::string, std::shared_ptr<Scene>> LoadedScenes;


		std::string TopScenePath = "";
	};
}