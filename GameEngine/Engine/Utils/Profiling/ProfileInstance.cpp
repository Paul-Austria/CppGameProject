#include "ProfileInstance.hpp"
#include <spdlog/spdlog.h>

#include <Engine/Renderer/ImGUI/imgui/imgui.h>
#include <Engine/Utils/StringHelpers.hpp>


GameEngine::ProfileInstance* GameEngine::ProfileInstance::instance = 0;

namespace GameEngine {

	ProfileInstance* ProfileInstance::GetInstance()
	{
		if (!instance)
		{
			instance = new ProfileInstance();
		}
		return instance;
	}


	void ProfileInstance::StartProfileSession(const std::string& name)
	{
		if (childProfiles.find(name) == childProfiles.end())
		{
			childProfiles[name] = ProcessTimeProfile(name);
		}
		childProfiles[name].Start();
	}
	float ProfileInstance::EndProfileSession(const std::string& name)
	{
		return childProfiles[name].Stop();
	}
	void ProfileInstance::Clear()
	{
		childProfiles.clear();
	}
	ProcessTimeProfile& ProfileInstance::GetProfile(const std::string& name)
	{
		return childProfiles[name];
	}
	void ProfileInstance::PrintDataToTerminal()
	{

		spdlog::info("===============ProfileOUTBEGIN=======================");
		for (auto it = childProfiles.begin(); it != childProfiles.end(); it++)
		{
			it->second.PrintData();
		}

		spdlog::info("===============ProfileOUTEND=========================");
	}
	void ProfileInstance::PrintImGUI()
	{

		for (auto it = childProfiles.begin(); it != childProfiles.end(); it++)
		{
			it->second.PrintImGUIData();
		}
	}
	std::map<std::string, ProcessTimeProfile>& ProfileInstance::GetAllProfiles()
	{
		return childProfiles;
	}
}