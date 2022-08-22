#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <Engine/Renderer/Renderable.hpp>
#include <memory>
#include "NativeScript.hpp"
#include <spdlog/spdlog.h>

namespace GameEngine
{
    enum TextureLoadType {
        nearest = GL_NEAREST,
        linear = GL_LINEAR,
    };
    struct Texture
    {
        unsigned int ID = 0;
        unsigned int width = 10, height = 10;
        std::string name = "";
        Texture() = default;
        Texture(unsigned int width, unsigned int height, unsigned char* data,std::string name="", TextureLoadType loadType = nearest)
        {
            this->name = name;
            this->width = width;
            this->height = height;
            // create Texture
            glGenTextures(1, &this->ID);
            glBindTexture(GL_TEXTURE_2D, this->ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, loadType);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, loadType);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            // set Texture wrap and filter modes
            // unbind texture
            glBindTexture(GL_TEXTURE_2D, 0);
        };
        void Bind()
        {
            glBindTexture(GL_TEXTURE_2D, this->ID);
        };
    };

    struct SubTexture
    {
        SubTexture() = default;
        SubTexture(Texture* mainTexture, unsigned int width, unsigned int height, float coordinates[], std::string name) : texture(mainTexture), width(width), height(height)
        {
            this->name = name;
            for (int i = 0; i < 8; i++)
            {
                textureCoordinates[i] = coordinates[i];
            }
        }
        unsigned int width, height;
        float textureCoordinates[8];
        std::string name = "";
        Texture* texture;
    };


    struct SubAnimation
    {
        SubAnimation() = default;
        SubAnimation(std::vector<SubTexture*> textures, float frameTime, bool repeat = false)
        {
            this->repeat = repeat;
            subTextures = textures;
            this->frameTime = 1.0f / frameTime;
        }
        std::vector<SubTexture*> subTextures;
        float frameTime;
        bool repeat = false;
    };
    struct Animator
    {
        int AddSubAnimation(std::string name, SubAnimation sub)
        {
            animations[name] = sub;
            return 0;
        }
        int SetCurrentAnimation(std::string name)
        {
            if (animations.find(name) != animations.end())
            {
                this->currentAnimation = animations[name];
                currentPosition = 0;
                timeSpend = 0;
                return 0;
            }
            return -1;
        }
        void Update(Renderable* renderable, float time)
        {
            timeSpend += time;
            if (timeSpend > currentAnimation.frameTime)
            {
                auto sub = currentAnimation.subTextures.at(currentPosition);
                renderable->SetSubTexture(sub);
                currentPosition++;
                if (currentPosition >= currentAnimation.subTextures.size())
                {
                    currentPosition = 0;
                }
                timeSpend = 0;
            }
        }
        std::unordered_map<std::string, SubAnimation> animations;
        SubAnimation currentAnimation;
        int currentPosition;
        float timeSpend;
    };




  

   
    struct NativeScriptHolder {
        std::shared_ptr<NativeScript> nativeScript = nullptr;

        NativeScriptHolder(std::shared_ptr<NativeScript> script) : nativeScript(script) {

        }

        ~NativeScriptHolder() {
        }
    };



} // namespace Engine
