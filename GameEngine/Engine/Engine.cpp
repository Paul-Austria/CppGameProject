#include "Engine.hpp"
#include <spdlog/spdlog.h>
#include <GameProjectConfig.h>

#include "Scene/Scene.hpp"
#include "Core/Window.hpp"
#include "Renderer/Renderer.hpp"
#include <Engine/Utils/Serialisation/ProjectSerialisation.hpp>
#include <Engine/Entities/Entity.hpp>
GameEngine::Engine* GameEngine::Engine::instance = 0;

namespace GameEngine {
    Engine::Engine()
    {
    }
    Engine::~Engine()
    {
        delete instance;
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
            float timeDiff = LastTime - glfwGetTime();
            LastTime = glfwGetTime();
            SceneUpdate(timeDiff);
#ifdef DEBUG
            DevelopUpdate(timeDiff);
#endif // DEBUG
            glfwSwapBuffers(Window::GetInstance()->GetWindow());
            glfwPollEvents();

        }
    }
    Engine* Engine::GetInstance()
    {
        if (!instance)
        {
            instance = new Engine();
            spdlog::info("New Engine created, Testing Version: {0}", GameProject_VERSION);
        }
        return instance;
    }

    void Engine::Reset()
    {
        //TODO
    }

    void Engine::LoadProject(const std::string& path)
    {
        currentProject = ProjectSerialisation::DeserializeProject(path);
    //    currentProject.SetCurrentScene(currentProject,TopScenePath);
        projectLoaded = true;

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

