#pragma once
#include <Engine/Entities/data/DataStructs.hpp>


namespace GameEngine
{

    class Renderable
    {
    public:
        Renderable() = default;

        Renderable(Texture* texture);
        Renderable(SubTexture* SubTexture);
        ~Renderable();
        void SetTexture(Texture* texture);
        void SetSubTexture(SubTexture* texture);
        unsigned int GetVAO();
        int GetWidth();
        int GetHeight();
        Texture* GetTexture();
    private:
        unsigned int VBO, VAO, EBO, TBO;
        Texture* texture;
        int width = 50, height = 50;
    };

}