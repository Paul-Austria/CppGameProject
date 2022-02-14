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
#include <Engine/Entities/data/DataStructs.hpp>


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


#ifdef DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGlErrorMessage, 0);

#endif // DEBUG


        sh = ShaderProgram("Resources/Shaders/Vertex.vs", "Resources/Shaders/Fragment.fs");


        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
 
        glGenRenderbuffers(1, &depthbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);


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
	void Renderer::BeginRender(CameraComponent& camera, Texture& renderTarget)
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        if (renderTarget.ID == 0)
        {
            glGenTextures(1, &renderTarget.ID);
        }
        currentTarget = renderTarget;
        glBindTexture(GL_TEXTURE_2D, renderTarget.ID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTarget.width, renderTarget.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


        glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderTarget.width, renderTarget.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);


        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTarget.ID, 0);
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers);


        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh.useShader();
        
        float width = renderTarget.width;
        float height = renderTarget.height;
        float aspect = 16/9;

        glm::mat4 projection = glm::ortho(-16.0f/9.0f, 16.0f/9.0f,
            9.0f/16.0f, -9.0f/16.0f,
            -1000.0f, 1000.0f);

        sh.setMat4("projection", projection);
        sh.setMat4("view", camera.GetViewMatrix());

	}
	void Renderer::RenderQuad(Renderable& renderable, const TransformComponent& TransformComponent, const CameraComponent& cameraComponent)
	{
        float width = currentTarget.width / 2.0f;
        float height = currentTarget.height / 2.0f;
        float maxPosX = cameraComponent.position.x + width + cameraComponent.pixelBuffer;
        float minPosX = cameraComponent.position.x - width - cameraComponent.pixelBuffer;
        float minPosY = cameraComponent.position.y - width - cameraComponent.pixelBuffer;
        float maxPosY = cameraComponent.position.y + height + cameraComponent.pixelBuffer;



        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,  TransformComponent.position/cameraComponent.zoom);
        model = glm::scale(model, glm::vec3(   ((TransformComponent.scale.x) * renderable.GetWidth() ) / cameraComponent.zoom / textureSize,( (TransformComponent.scale.y) * renderable.GetHeight()) / cameraComponent.zoom / textureSize, 1.0f));
        sh.setMat4("model", model);
        sh.setBool("useColor", renderable.UseColor());
        sh.setVec4("inColor", renderable.GetColor());

        glBindVertexArray(renderable.GetVAO());
        if (renderable.GetTexture() != nullptr )
        {
            lastTexture = renderable.GetTexture()->ID;
            renderable.GetTexture()->Bind();
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

	}
	void Renderer::EndRender()
	{
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


    void APIENTRY Renderer::OpenGlErrorMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
        const GLchar* message, const void* userParam)
    {
        const char* _source;
        const char* _type;
        const char* _severity;

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

        
       
    }

}