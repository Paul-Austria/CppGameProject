#include "TextureLoader.hpp"
#include <corecrt_io.h>
#include <filesystem>
#include <fstream>
#include "json/json.hpp"

#include <Engine/ResourceManagement/TextureResourceManager.hpp>
#include <stdexcept>
using json = nlohmann::json;

namespace GameEngine
{
    int TextureLoader::LoadTexturesByJson(const std::string& path)
    {
        int counter = 0;
        std::string extension = std::filesystem::path(path).extension().string();
        if (extension != ".json")
        {
            spdlog::error(path + " is not json");
            return -1;
        }
        auto manager = TextureResourceManager::GetInstance();
        std::string parentPath = std::filesystem::path(path).parent_path().string() + "\\";

        std::string fullPath = std::filesystem::absolute(path).string();

        std::ifstream i(path);
        if (!i) {
            spdlog::error("Failed to read file");
            return -1;
        }
        std::stringstream buffer;
        buffer << i.rdbuf();
        std::string buf = buffer.str();
        auto j = json::parse(buf);
        auto x = j["Textures"];
        for (auto sub : x)
        {
            manager->LoadTexture(sub["name"].get<std::string>(), parentPath + sub["path"].get<std::string>());
            counter++;
        }
        spdlog::info("{0} Textures loaded", counter);
        counter = 0;
        x = j["SubTextures"];
        for (auto sub : x)
        {
            auto texture = manager->GetTexture(sub["texture"].get<std::string>());
            std::string name = sub["name"].get<std::string>();
            auto pos = sub["pos"];
            auto size = sub["size"];
            manager->AddSubTexture(name, texture, { size[0].get<float>(), size[1].get<float>() },
                { pos[0].get<float>(), pos[1].get<float>() });
            counter++;
        }
        spdlog::info("{0} SubTextures loaded", counter);
        counter = 0;
        return 0;
    }

} // namespace Engine