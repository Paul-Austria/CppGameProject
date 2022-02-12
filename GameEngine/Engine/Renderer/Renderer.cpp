#include "Renderer.hpp"

#include <spdlog/spdlog.h>
#include <GL/glew.h>
#include "Engine/Renderer/ImGUI/imgui/imgui.h"
#include "Engine/Renderer/ImGUI/imgui/backends/imgui_impl_glfw.h"
#include "Engine/Renderer/ImGUI/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Engine/Core/Window.hpp"
#include <filesystem>

#include <glm/gtc/matrix_transform.hpp>


#include "ImGUI/ImGUIStyling.hpp"

GameEngine::Renderer* GameEngine::Renderer::instance = 0;
namespace GameEngine {
	Renderer::Renderer()
	{
	}
	Renderer::~Renderer()
	{
        glDeleteShader(sh.getShaderProgram());

	}
	Renderer* Renderer::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new Renderer();
		}
		return instance;
	}
	void Renderer::Init()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		// Setup Dear ImGui style
		ImGUIStyling::BasicStyle();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(Window::GetInstance()->GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 130");

        sh = ShaderProgram("D:/PR/GameProject/Resources/Shaders/Vertex.vs", "D:/PR/GameProject/Resources/Shaders/Fragment.fs");
	}
	void Renderer::StartImGUI()
	{
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

	}
	void Renderer::EndImGUI()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	}
	void Renderer::BeginRender(CameraComponent& camera)
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh.useShader();
        
        float width = Window::GetInstance()->GetWidth() / camera.zoom;
        float height = Window::GetInstance()->GetHeigth() / camera.zoom;;
        glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -10000.0f, 10000.0f);
        sh.setMat4("projection", projection);
        sh.setMat4("view", camera.GetViewMatrix());

	}
	void Renderer::RenderQuad(Renderable& renderable, const TransformComponent& TransformComponent, const CameraComponent& cameraComponent)
	{
        float width = Window::GetInstance()->GetWidth() / 2.0f / cameraComponent.zoom;
        float height = Window::GetInstance()->GetHeigth() / 2.0f / cameraComponent.zoom;
        float maxPosX = cameraComponent.position.x + width + cameraComponent.pixelBuffer;
        float minPosX = cameraComponent.position.x - width - cameraComponent.pixelBuffer;
        float minPosY = cameraComponent.position.y - width - cameraComponent.pixelBuffer;
        float maxPosY = cameraComponent.position.y + height + cameraComponent.pixelBuffer;
        if (TransformComponent.position.x < maxPosX && TransformComponent.position.x > minPosX && TransformComponent.position.y < maxPosY && TransformComponent.position.y > minPosY)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(TransformComponent.position.x + width, TransformComponent.position.y + height, TransformComponent.position.z));
            model = glm::scale(model, glm::vec3(renderable.GetWidth(), renderable.GetHeight(), 1.0f));
            sh.setMat4("model", model);

            glBindVertexArray(renderable.GetVAO());
            lastTexture = renderable.GetTexture()->ID;
            renderable.GetTexture()->Bind();

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

	}
	void Renderer::EndRender()
	{
	}


    void APIENTRY Renderer::OpenGlErrorMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
        const GLchar* message, const void* userParam)
    {
    /*    char* _source;
        char* _type;
        char* _severity;

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            _source = "UNKNOWN";
            break;

        default:
            _source = "UNKNOWN";
            break;
        }

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;

        default:
            _type = "UNKNOWN";
            break;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;

        default:
            _severity = "UNKNOWN";
            break;
        }

        printf("%d: %s of %s severity, raised from %s: %s\n", id, _type, _severity, _source, message);

        
        */
    }

}