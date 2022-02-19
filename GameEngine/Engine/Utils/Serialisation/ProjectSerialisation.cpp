#include "ProjectSerialisation.hpp"
#include <Engine/Scene/ProjectData.hpp>

#include <filesystem>
#include <fstream>

#include <ctime>
#include <iostream>
#include <sstream>

#include <Engine/Scene/Scene.hpp>

#include <GLFW/glfw3.h>


#include <list>

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

		std::list<json> subObjects = std::list<json>();
		for (auto scene : projectToSave.LoadedScenes)
		{
			json jb;
			jb["SceneFile"] = scene.first + ".scjson";
			std::ofstream outScene(path +"/" + scene.first + ".scjson");
			std::string out = SerializeScene(scene.second).dump();
			outScene << out;
			subObjects.push_back(jb);
		}
		j["Scenes"] = subObjects;

		std::string output = j.dump();
		std::ofstream file(path + "/Project.json");
		file << output;
		return 0;
	}
	json ProjectSerialisation::SerializeScene(std::shared_ptr<Scene> scene)
	{
		json j;
		j["name"] = scene->sceneName;
		auto view = scene->registry.view<TagComponent>();

		std::list<json> subObjects = std::list<json>();

		for (auto ent : view) {
			json subObject;
			std::string tag = scene->registry.get<TagComponent>(ent).Tag;

			subObject["name"] = tag;


			json positionData;


			auto transform = scene->registry.get<TransformComponent>(ent);

			json pos;
			pos["X"] = transform.position.x;
			pos["Y"] = transform.position.y;
			pos["Z"] = transform.position.z;

			positionData["Position"] = pos;


			positionData["Rotation"] = transform.rotation;

			json scale;

			scale["X"] = transform.scale.x;
			scale["Y"] = transform.scale.y;


			positionData["Scale"] = scale;

			subObject["Transformation"] = positionData;


			if (scene->registry.all_of<Renderable>(ent))
			{
				json renderable;

				auto rend = scene->registry.get<Renderable>(ent);
				renderable["Height"] = rend.GetHeight();
				renderable["Width"] = rend.GetWidth();
				json color;

				color["R"] = rend.GetColor().r;
				color["G"] = rend.GetColor().g;
				color["B"] = rend.GetColor().b;
				color["A"] = rend.GetColor().a;

				renderable["Color"] = color;
				renderable["UseColor"] = rend.UseColor();
				
				subObject["Renderable"] = renderable;
			}

			subObjects.push_back(subObject);
			subObject.clear();
		}
		j["entities"] = subObjects;
		return j;
	}
}