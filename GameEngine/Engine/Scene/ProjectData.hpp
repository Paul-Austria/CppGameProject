#pragma once

#include <map>
#include <string>
#include <list>
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

		std::string& GetPath() { return Path; }
		void LoadScene(const std::string& name);

		void SetCurrentScene(const std::string& name);
	

		int UnloadScene(const std::string& name);
		std::shared_ptr<Scene> CreateNewScene(const std::string& name);


		void SetTopScene(const std::string& topScene) { this->TopSceneName = topScene; }
		std::string GetTopScene() { return TopSceneName; }
	protected:
		void SetPath(const std::string& path) { this->Path = path; }
	private:

		friend class Engine;
		friend class ProjectSerialisation;

		std::string Path = "D:/PR/GameEngineTesting/TestProject";
		std::string Name = "New_Project";
		//Name, Path
		std::map<std::string, std::string> ResourceIncludes;
		//Name, Path
		std::list<std::string> Scenes;
		std::map<std::string, std::shared_ptr<Scene>> LoadedScenes;


		std::string TopSceneName = "New_Scene";
	};
}