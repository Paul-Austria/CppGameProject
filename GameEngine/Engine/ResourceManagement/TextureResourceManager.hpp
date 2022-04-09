#pragma once
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <Engine/Entities/data/DataStructs.hpp>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <list>

namespace GameEngine
{
    class EditorView;
    class TextureResourceManager
    {
    public:
        TextureResourceManager();
        ~TextureResourceManager();
        static TextureResourceManager* GetInstance();
        int LoadTexture(std::string name, std::string path, TextureLoadType type = nearest);
        int AddSubTexture(std::string name, Texture* texture, glm::vec2 size, glm::vec2 pos);
        bool TextureExists(const std::string& search);
        SubTexture* GetSubTexture(const std::string& search);
        Texture* GetTexture(const std::string& search);

        std::vector<std::string> GetFittingTextureNames(const std::string& search);


    private:

        friend class EditorView;

        std::unordered_map<std::string, Texture> textures;
        std::unordered_map<std::string, SubTexture> subTextures;


        static TextureResourceManager* instance;
    };

}
