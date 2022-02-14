#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position,0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));

			glm::mat4 m_ViewMatrix = glm::inverse(transform);
			return m_ViewMatrix;
		}
	};
}