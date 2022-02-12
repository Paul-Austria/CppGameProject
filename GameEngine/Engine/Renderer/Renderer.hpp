#include <Engine/Entities/BaseComponents.hpp>
#include "Renderable.hpp"
#include "Shaders/ShaderProgram.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GameEngine {
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		static Renderer* GetInstance();


		void Init();
		void StartImGUI();
		void EndImGUI();


		void BeginRender(CameraComponent& camera);
		void RenderQuad(Renderable& renderable, const TransformComponent& TransformComponent,const CameraComponent& cameraComponent);
		void EndRender();



	private:
		static Renderer* instance;
		ShaderProgram sh;
		unsigned int lastTexture = -1;




		static void APIENTRY OpenGlErrorMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* userParam);

	};
}