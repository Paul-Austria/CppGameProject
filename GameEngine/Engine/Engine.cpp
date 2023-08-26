#include "Engine.hpp"
#include <spdlog/spdlog.h>
#include <GameProjectConfig.h>

#include "Scene/Scene.hpp"
#include "Core/Window.hpp"
#include "Renderer/Renderer.hpp"
#include <Engine/Utils/Serialisation/ProjectSerialisation.hpp>
#include <Engine/Entities/Entity.hpp>
#include <Engine/ResourceManagement/TextureResourceManager.hpp>
#include <Engine/Utils/Profiling/ProfileInstance.hpp>

//#define SOL_ALL_SAFETIES_ON 1
//#include <sol/sol.hpp>
//#include <Engine/sol/assert.hpp>
#include <iostream>
#include <memory>
std::unique_ptr<GameEngine::Engine> GameEngine::Engine::instance = 0;

namespace GameEngine {
    Engine::Engine()
    {
    }
    Engine::~Engine()
    {
    //    delete instance;
    }
    void Engine::Init(int widht, int height,const std::string& title , const std::string& ProjectPath, bool isFullScreen)
    {
        Window* window = Window::GetInstance();
   
        window->Init(widht, height, title.c_str(), isFullScreen);
        Renderer::GetInstance()->Init();
        if (ProjectPath == "")
        {
            currentProject = ProjectData();
            currentScene = currentProject.CreateNewScene("New_Scene");
        }
        else
        {
            LoadProject(ProjectPath);
        }
    }

    void Engine::SetCurrentScene(std::shared_ptr<Scene> scene)
    {
        this->currentScene = scene;
    }

    void Engine::Loop()
    {
        LastTime = glfwGetTime();
        while (isRunning && !glfwWindowShouldClose(Window::GetInstance()->GetWindow()))
        {
            if (toChangeScene)
            {
                toChangeScene = false;
                currentScene->ChangeStatus(Stopped);
                currentScene = currentProject.LoadedScenes[toChangeSceneName];
//                currentScene->ChangeStatus(Running);
            }
            ProfileInstance::GetInstance()->StartProfileSession("Loop");
            float timeDiff = LastTime - glfwGetTime();
            LastTime = glfwGetTime();
            SceneUpdate(timeDiff);
#ifdef DEBUG
            DevelopUpdate(timeDiff);
#endif // DEBUG
            glfwSwapBuffers(Window::GetInstance()->GetWindow());
            glfwPollEvents();
            ProfileInstance::GetInstance()->EndProfileSession("Loop");
        //    ProfileInstance::GetInstance()->PrintDataToTerminal();
        }
    }
    GameEngine::Engine* Engine::GetInstance()
    {
        if (!instance)
        {
            instance = std::make_unique<Engine>();
            spdlog::info("New Engine created, Testing Version: {0}", GameProject_VERSION);
        }
        Engine* ptr =  instance.get();
        return ptr;
    }

    void Engine::Reset()
    {
        TextureResourceManager::GetInstance()->Clear();
        currentProject = ProjectData();
        currentProject.CreateNewScene("FirstScene");
        currentProject.SetCurrentScene("FirstScene", false);
    }

    void Engine::LoadProject(const std::string& path)
    {
        currentProject = ProjectSerialisation::DeserializeProject(path);
        currentProject.SetCurrentScene(currentProject.GetTopScene(), false);
        projectLoaded = true;

    }

    Input* Engine::GetInput()
    {
        return &input;
    }

    void Engine::PrepareSceneChange(const std::string& nextScene)
    {
        toChangeScene = true;
        toChangeSceneName = nextScene;
    }

    void Engine::DevelopUpdate(float deltaTime)
    {
        if (currentScene != nullptr)
        {
            currentScene->EditorUpdate(deltaTime);
        }
    }
    void Engine::SceneUpdate(float deltaTime)
    {
        if (currentScene != nullptr)
        {
            currentScene->BackgroundUpdate(deltaTime,isRunning);
        }
    }


}

