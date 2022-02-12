#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "data/DataStructs.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>



namespace GameEngine {
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};


	struct TransformComponent
	{
		glm::vec3 position{ 0, 0, 0 };
		float rotation = 0.0f;
		glm::vec2 scale{ 1, 1 };
	};


	struct CameraComponent {
		float zoom = 0.5;
		glm::vec2 position{ 0,0 };
		unsigned int pixelBuffer = 120;


		CameraComponent() = default;
		~CameraComponent() = default;

		glm::mat4 GetViewMatrix()
		{
			glm::mat4 view = glm::mat4(1.0f);
			return glm::translate(view, glm::vec3(-position, 100.0f));
		}
	};
}