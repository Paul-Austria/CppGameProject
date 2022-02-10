#include <string>
#include <glm/glm.hpp>




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
}