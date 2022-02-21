#include "ProjectSerialisation.hpp"
#include <Engine/Scene/ProjectData.hpp>

#include <filesystem>
#include <fstream>

#include <ctime>
#include <iostream>
#include <sstream>

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entities/Entity.hpp>

#include <GLFW/glfw3.h>


#include <list>

using json = nlohmann::json;

namespace GameEngine {
#pragma region Serialize

	int ProjectSerialisation::SerializeProject(ProjectData& projectToSave)
	{
		std::string path = projectToSave.GetPath();

		json j;

		j["name"] = projectToSave.GetName();
		j["topScene"] = projectToSave.GetTopScene();
		std::time_t t = std::time(0);
		std::stringstream ss;

		ss << std::localtime(&t);

		j["saveDate"] = ss.str();

		std::list<json> subObjects = std::list<json>();
		for (auto scene : projectToSave.LoadedScenes)
		{
			json jb;
			jb["sceneFile"] = scene.first;
			std::ofstream outScene(path + "/" + scene.first + ".scjson");
			std::string out = SerializeScene(scene.second).dump();
			outScene << out;
			subObjects.push_back(jb);
		}
		j["scenes"] = subObjects;

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
			pos["x"] = transform.position.x;
			pos["y"] = transform.position.y;
			pos["z"] = transform.position.z;

			positionData["position"] = pos;


			positionData["rotation"] = transform.rotation;

			json scale;

			scale["x"] = transform.scale.x;
			scale["y"] = transform.scale.y;


			positionData["scale"] = scale;

			subObject["transformation"] = positionData;


			if (scene->registry.all_of<Renderable>(ent))
			{
				json renderable;

				auto rend = scene->registry.get<Renderable>(ent);
				renderable["height"] = rend.GetHeight();
				renderable["width"] = rend.GetWidth();
				json color;

				color["r"] = rend.GetColor().r;
				color["g"] = rend.GetColor().g;
				color["b"] = rend.GetColor().b;
				color["a"] = rend.GetColor().a;

				renderable["color"] = color;
				renderable["useColor"] = rend.UseColor();

				subObject["renderable"] = renderable;
			}

			subObjects.push_back(subObject);
			subObject.clear();
		}
		j["entities"] = subObjects;
		return j;
	}
	
#pragma endregion
#pragma region Desiarilze
	ProjectData ProjectSerialisation::DeserializeProject(const std::string& path){

		ProjectData data = ProjectData();
		data.SetPath(path);

		std::string output = GetFileData(path + "/Project.json");

		json j = json::parse(output);


		data.SetName(j["name"]);
		data.SetTopScene(j["topScene"]);

		for(auto j : j["scenes"])
		{
			data.Scenes.push_back(j["sceneFile"]);
		}
		data.LoadedScenes[j["topScene"]] = LoadScene(path+"/"+data.GetTopScene()+".scjson");
		return data;
	}


	std::string ProjectSerialisation::GetFileData(const std::string& path)
	{
		std::ifstream file(path);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
	std::shared_ptr<Scene> ProjectSerialisation::LoadScene(const std::string& path)
	{
		std::string fileData = GetFileData(path);
		json j = json::parse(fileData);

		auto scene = std::make_shared<Scene>();

		scene->sceneName = j["name"];

		for (auto enti : j["entities"])
		{
			auto ent = scene->CreateEntity(enti["name"]);
		}

		return scene;
	}
#pragma endregion

}