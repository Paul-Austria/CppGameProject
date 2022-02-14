#pragma once

#include <Engine/Entities/BaseComponents.hpp>

#include "Shaders/ShaderProgram.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderable.hpp"


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


		void BeginRender(CameraComponent& camera, Texture& target);
		void RenderQuad(Renderable& renderable, const TransformComponent& TransformComponent,const CameraComponent& cameraComponent);
		void EndRender();

		void SetUnitSize(int textureSize) { this->textureSize = textureSize; }
		int  GetUnitSize() { return textureSize; }

	private:
		static Renderer* instance;
		ShaderProgram sh;
		unsigned int lastTexture = -1;

		unsigned int framebuffer = 0;
		Texture currentTarget;
		unsigned int depthbuffer = 0;
		int textureSize = 128;


		static void APIENTRY OpenGlErrorMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* userParam);

	};
}