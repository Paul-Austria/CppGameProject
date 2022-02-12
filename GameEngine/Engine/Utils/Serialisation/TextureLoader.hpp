#pragma once
#include <string>
#include <spdlog/spdlog.h>

namespace GameEngine
{
    class TextureLoader
    {
    public:
        static int LoadTexturesByJson(const std::string& path);
    private:
    };

}
