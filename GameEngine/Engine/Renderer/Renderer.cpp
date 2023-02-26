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
//		glEnable(GL_CULL_FACE);
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
        screenShader = ShaderProgram("Resources/Shaders/screenShader.vs", "Resources/Shaders/screenShader.fs");


        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        /*
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
 
        glGenRenderbuffers(1, &depthbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

        */
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
    //    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*
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

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        */

        float width = Window::GetInstance()->GetWidth();
        float height = Window::GetInstance()->GetHeigth();
        float aspect = width/height;
        printf("%f, %f \n", camera.zoom, height);
        /*
                glm::mat4 projection = glm::ortho(
            -1.0f / camera.zoom, aspect / camera.zoom,
            1.0f/ camera.zoom, -height/width / camera.zoom,
            -1000.0f, 1000.0f);
        */
        static float zoom = 1;
        sh.useShader();

        zoom -= 0.01f;
        float target_width = width;
        float target_height = height;
        float A = (target_width / target_height)/100; // target aspect ratio 
        float V = A ;

        glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1000.0f, 1000.0f);


        // ... calculate V as above
        if (true) {
            // wide viewport, use full height
            projection = glm::ortho(-V / A * target_width / 100.0f, V / A * target_width / 100.0f, -target_height / 100.0f, target_height / 100.0f, -1000.0f, 1000.0f);
        }
        else {
            // tall viewport, use full width
           projection = glm::ortho(-target_width / 100.0f, target_width / 100.0f, -A / V * target_height / 100.0f, A / V * target_height / 100.0f, -1000.0f, 1000.0f);
        }




       

        sh.setMat4("projection", projection);
    //   sh.setMat4("view", camera.GetViewMatrix());

        

        float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        // world space positions of our cubes
        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,  0.0f,  0.0f),
            glm::vec3(2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f,  2.0f, -2.5f),
            glm::vec3(1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        

        static float x = 3;
 //       x += 0.1f;
        // activate shader
        sh.useShader();
        // create transformations
//        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);



        glm::vec3 cameraForword = { 1,0,0 };
        glm::vec3 CameraRight = { 0,0,1 };
        glm::vec3 CameraUp = { 0,1,0 };
        glm::vec3 position = glm::vec3(0, 0, 0);

        /*

        float horizontalAngle = 3.14f;
        float verticalAngle = 0.0f;


        glm::vec3 direction(
            cos(verticalAngle)* sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle)* cos(horizontalAngle));

        glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f));

        glm::vec3 up = glm::cross(right, direction);
        */
        glm::mat4 viewMatrix = glm::mat4(1);

        viewMatrix = glm::lookAt(
            position,
            position + cameraForword,
            CameraUp);

        // pass transformation matrices to the shader
 //       sh.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        sh.setMat4("view", viewMatrix);


        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, {1,1,1});


            sh.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
        glm::vec3 post = { TransformComponent.position.z, TransformComponent.position.y, TransformComponent.position.x};
        model = glm::translate(model, post);
        float angle = 20.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        sh.setMat4("model", model);
        sh.setMat4("model", model);
        sh.setBool("useColor", renderable.UseColor());
        sh.setVec4("inColor", renderable.GetColor());

        glBindVertexArray(renderable.GetVAO());
        if (renderable.GetTexture() != nullptr)
        {
            lastTexture = renderable.GetTexture()->ID;
            renderable.GetTexture()->Bind();
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

	}
	void Renderer::EndRender()
	{
        /*
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    
        screenShader.useShader();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, currentTarget.ID);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0,6);


    */
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