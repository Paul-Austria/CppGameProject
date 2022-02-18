#pragma once

namespace GameEngine {
	class ProjectData;
	class ProjectSerialisation
	{
	public:
		ProjectSerialisation() = default;
		~ProjectSerialisation() = default;



		static int SerializeProject(ProjectData& projectToSave);

	private:

	};
}
