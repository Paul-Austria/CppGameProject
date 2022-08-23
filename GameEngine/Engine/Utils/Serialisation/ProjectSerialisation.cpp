#include "ProjectSerialisation.hpp"
#include <Engine/Scene/ProjectData.hpp>
#include <Engine/ResourceManagement/TextureResourceManager.hpp>
#include <filesystem>
#include <fstream>

#include <ctime>
#include <iostream>
#include <sstream>

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entities/Entity.hpp>
#include <Engine/Entities/data/LuaScript.hpp>

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


			if (scene->registry.all_of<CameraComponent>(ent))
			{
				json camera;
				auto cam = scene->registry.get<CameraComponent>(ent);
				camera["active"] = cam.active;
				camera["zoom"] = cam.zoom;
				subObject["camera"] = camera;
			}
			if (scene->registry.all_of<LuaScript>(ent))
			{
				LuaScript& script = scene->registry.get<LuaScript>(ent);
				json lScript;

				lScript["path"] = script.scriptPath;
				subObject["luaScript"] = lScript;
			}
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
				renderable["usesSubTexture"] = rend.UsesSubTexture();

				if (rend.UsesSubTexture())
				{
					renderable["subTextureName"] = rend.GetSubTextureName();
				}
				if (rend.GetTexture() != nullptr)
				{
					renderable["texture"] = rend.GetTexture()->name;
				}
				else
				{
					renderable["texture"] = "";
				}

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

		ProjectData data = ProjectData(path);

		std::string output = GetFileData(path + "/Project.json");

		json j = json::parse(output);


		data.SetName(j["name"]);
		data.SetTopScene(j["topScene"]);

		for(auto j : j["scenes"])
		{
			data.Scenes.push_back(j["sceneFile"]);
		}
		Engine::GetInstance()->currentProject.Path = path;
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
			TransformComponent& transformation = ent.GetComponent<TransformComponent>();

			transformation.position.x = enti["transformation"]["position"]["x"].get<float>();
			transformation.position.y = enti["transformation"]["position"]["y"].get<float>();
			transformation.position.z = enti["transformation"]["position"]["z"].get<float>();

			transformation.rotation = enti["transformation"]["rotation"].get<float>();

			transformation.scale.x = enti["transformation"]["scale"]["x"].get<float>();
			transformation.scale.y = enti["transformation"]["scale"]["y"].get<float>();

			if (enti.contains("camera")) {
				CameraComponent& camera = ent.AddComponent<CameraComponent>();
				camera.active = enti["camera"]["active"].get<bool>();
				camera.zoom = enti["camera"]["zoom"].get<float>();
			}
			if (enti.contains("renderable"))
			{
				Renderable& renderable = ent.AddComponent<Renderable>(50,50);
				json rend = enti["renderable"];

				renderable.SetUseColor(rend["useColor"].get<bool>());
				renderable.SetWidth(rend["width"].get<int>());
				renderable.SetHeight(rend["height"].get<int>());

				glm::vec4 col = { 
					rend["color"]["r"].get<float>(),
					rend["color"]["g"].get<float>(),
					rend["color"]["b"].get<float>(),
					rend["color"]["a"].get<float>() };

				renderable.SetColor(col);
				if (rend["usesSubTexture"].get<bool>())
				{
					renderable.UsesSubTexture();
					renderable.SetSubTexture(TextureResourceManager::GetInstance()->GetSubTexture(rend["subTextureName"]));
				}
				else if (rend["texture"] != "")
				{
					renderable.SetTexture(TextureResourceManager::GetInstance()->GetTexture(rend["texture"]));
				}
				
			}

			if (enti.contains("luaScript"))
			{
				json lscript = enti["luaScript"];
				LuaScript& luaScript = ent.AddComponent<LuaScript>(scene->luaHandler.GenerateScript(lscript["path"], ent));
			}
		}
		scene->status = Stopped;
		return scene;
	}
#pragma endregion

}