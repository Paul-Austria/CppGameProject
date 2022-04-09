#pragma once
#include <glm/glm.hpp>
#include <string>
namespace GameEngine
{
    struct Texture;
    struct SubTexture;

    class Renderable
    {
    public:
        Renderable();
        Renderable(int width, int height);
        Renderable(Texture* texture);
        Renderable(SubTexture* SubTexture);
        ~Renderable();
        void SetTexture(Texture* texture);
        void SetSubTexture(SubTexture* texture);
        std::string GetSubTextureName() { return subTextureName; }
        unsigned int GetVAO();
        int GetWidth();
        int GetHeight();


        void SetHeight(int h) { this->height = h; }
        void SetWidth(int w) { this->width = w; }

        glm::vec4 GetColor() { return color; }
        void SetColor(glm::vec4 col) { this->color = col; }

        bool UseColor() { return useColor; }
        void SetUseColor(bool useColor) { this->useColor = useColor; }

        Texture* GetTexture();

        bool UsesSubTexture() { return useSubTexture;  }
    private:
        unsigned int VBO, VAO, EBO, TBO;
        Texture* texture = nullptr;
        int width = 50, height = 50;
        
        glm::vec4 color = {0,0,0,1};

        std::string subTextureName = "";

        bool useColor = false;
        bool useSubTexture = false;
    };

}