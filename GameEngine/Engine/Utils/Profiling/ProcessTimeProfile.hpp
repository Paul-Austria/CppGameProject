#pragma once

#include <map>
#include <string>
namespace GameEngine {
	class ProcessTimeProfile {
	public:
		ProcessTimeProfile(const std::string& name);
		~ProcessTimeProfile();
		ProcessTimeProfile() = default;


		void Start();
		float Stop();

		float GetProcessTime() { return StopTime - StartTime; }


		void StartChildProfile(const std::string& name);
		float EndChildProfile(const std::string& name);
		void ClearChildren();

		void PrintData();

		ProcessTimeProfile& GetChildProfile(const std::string& name);
		
		std::map<std::string, ProcessTimeProfile>& GetAllChildProfiles();
	private:
		float StartTime;
		float StopTime;
		std::string name;

		std::map<std::string,ProcessTimeProfile> childProfiles;
	
		float stopped;

		friend class ProcessTimeProfile;
		friend class ProfileInstance;
	};
}