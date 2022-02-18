#include "ProjectSerialisation.hpp"
#include "json/json.hpp"
#include <Engine/Scene/ProjectData.hpp>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>

#include <ctime>
#include <iostream>
#include <sstream>


using json = nlohmann::json;

namespace GameEngine {
	int ProjectSerialisation::SerializeProject(ProjectData& projectToSave)
	{
		std::string path = projectToSave.GetPath();

		json j;

		j["name"] = projectToSave.GetName();

		std::time_t t = std::time(0);  
		std::stringstream ss;

		ss << std::localtime(&t);

		j["saveDate"] = ss.str();

		std::string output = j.dump();

		std::ofstream file(path + "/Project.json");
		file << output;
		return 0;
	}
}