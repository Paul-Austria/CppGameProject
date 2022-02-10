#pragma once
#include <map>
#include "ProcessTimeProfile.hpp"
namespace GameEngine {
	class ProfileInstance
	{
	public:
		ProfileInstance() {};
		~ProfileInstance(){};


		void StartProfileSession(const std::string& name);
		float EndProfileSession(const std::string& name);
		void Clear();
		ProcessTimeProfile& GetProfile(const std::string& name);

		void PrintDataToTerminal();

		std::map<std::string, ProcessTimeProfile>& GetAllProfiles();


		static ProfileInstance* GetInstance();


	private:
		std::map<std::string, ProcessTimeProfile> childProfiles;
		static ProfileInstance* instance;

	};
}


