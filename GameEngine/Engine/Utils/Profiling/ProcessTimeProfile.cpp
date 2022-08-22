#include "ProcessTimeProfile.hpp"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>


#include <Engine/Renderer/ImGUI/imgui/imgui.h>
#include <Engine/Utils/StringHelpers.hpp>
#include <sstream>

namespace GameEngine {
	ProcessTimeProfile::ProcessTimeProfile(const std::string& name)
	{
		this->name = name;
	}
	ProcessTimeProfile::~ProcessTimeProfile()
	{
	}
	void ProcessTimeProfile::Start()
	{
		StartTime = glfwGetTime();
		stopped = false;
	}
	float ProcessTimeProfile::Stop()
	{
		if (!stopped)
		{
			StopTime = glfwGetTime();
			stopped = true;
			for (auto it = childProfiles.begin(); it != childProfiles.end(); it++)
			{
				it->second.Stop();
			}
		}

		return GetProcessTime();
	}
	void ProcessTimeProfile::StartChildProfile(const std::string& name)
	{
		if (childProfiles.find(name) == childProfiles.end())
		{
			childProfiles[name] = ProcessTimeProfile(name);
		}

		childProfiles[name].Start();
	}
	float ProcessTimeProfile::EndChildProfile(const std::string& name)
	{
		if (childProfiles.find(name) == childProfiles.end())
		{
			return -1;
		}
		childProfiles[name].Stop();
		return childProfiles[name].GetProcessTime();
	}

	void ProcessTimeProfile::ClearChildren()
	{
		childProfiles.clear();
	}
	void ProcessTimeProfile::PrintData()
	{
	
		spdlog::info("{0} took: {1} ms", name, GetProcessTime());
		if (childProfiles.size() > 0)
		{

			spdlog::info("Children of {0}", name);
			for (auto it = childProfiles.begin(); it != childProfiles.end(); it++)
			{
				it->second.PrintData();
			}
			spdlog::info("End ofChildren of {0}", name);
		}

	}
	void ProcessTimeProfile::PrintImGUIData()
	{
		std::stringstream ss;
		ss << name << " took " << GetProcessTime();
		if (ImGui::CollapsingHeader(ss.str().c_str())) {
			if (childProfiles.size() > 0)
			{
				for (auto it = childProfiles.begin(); it != childProfiles.end(); it++)
				{
					it->second.PrintImGUIData();
				}
			}
		}
	}
	ProcessTimeProfile& ProcessTimeProfile::GetChildProfile(const std::string& name)
	{
		return childProfiles[name];
	}
	std::map<std::string, ProcessTimeProfile>& ProcessTimeProfile::GetAllChildProfiles()
	{
		return childProfiles;
	}
}