#pragma once
#include <string>
#include <memory>
#include "json/json.hpp"
using json = nlohmann::json;

namespace GameEngine {
	class ProjectData;
	class Scene;
	class ProjectSerialisation
	{
	public:
		ProjectSerialisation() = default;
		~ProjectSerialisation() = default;



		static int SerializeProject(ProjectData& projectToSave);

		static ProjectData DeserializeProject(const std::string& name);
		static std::shared_ptr<Scene> LoadScene(const std::string& path);

	private:
		static json SerializeScene(std::shared_ptr<Scene> scene);

		static std::string GetFileData(const std::string& path);

	};
}
