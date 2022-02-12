#include "TextureResourceManager.hpp"
#include <spdlog/spdlog.h>
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <Engine/Utils/stb/stb_image.hpp>

#include <filesystem>
GameEngine::TextureResourceManager* GameEngine::TextureResourceManager::instance = 0;

namespace GameEngine
{
    TextureResourceManager::TextureResourceManager()
    {
    }
    TextureResourceManager::~TextureResourceManager()
    {
    }

    TextureResourceManager* TextureResourceManager::GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new TextureResourceManager();
        }

        return instance;
    }

    int TextureResourceManager::LoadTexture(std::string name, std::string imagePath, TextureLoadType type)
    {
        std::string directPath = std::filesystem::absolute(imagePath).string();

        int width, height, nrChannels;
        unsigned char* data = stbi_load(directPath.c_str(), &width, &height, &nrChannels, 0);
        if (!data)
        {
            spdlog::error("Image file not found at: {0}", directPath);
            return -1;
        }
        Texture texture = Texture(width, height, data,type);
        textures[name] = texture;
        stbi_image_free(data);

        return 0;
    }
    Texture* TextureResourceManager::GetTexture(std::string name)
    {
        return &textures[name];
    }

    int TextureResourceManager::AddSubTexture(std::string name, Texture* texture, glm::vec2 size, glm::vec2 pos)
    {
        float width = size.x;
        float height = size.y;

        float textureX = 1 / (((float)texture->width) / width);
        float textureY = 1 / (((float)texture->height) / height);
        float coords[8];
        coords[0] = textureX * pos.x + textureX;
        coords[1] = textureY * pos.y + textureY;
        coords[2] = textureX * pos.x + textureX;
        coords[3] = textureY * pos.y;
        coords[4] = textureX * pos.x;
        coords[5] = textureY * pos.y;
        coords[6] = textureX * pos.x;
        coords[7] = textureY * pos.y + textureY;
        SubTexture subTexture = SubTexture(texture, width, height, coords);
        subTextures[name] = subTexture;
        return 0;
    }
    SubTexture* TextureResourceManager::GetSubTexture(std::string name)
    {
        return &subTextures[name];
    }


    //  std::unordered_map<std::string, Texture> textures;
} // namespace Engine
