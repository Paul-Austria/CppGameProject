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

	private:
		static json SerializeScene(std::shared_ptr<Scene> scene);
	};
}
