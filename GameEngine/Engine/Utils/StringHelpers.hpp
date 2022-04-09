#pragma once
#include <string>
#include <vector>
namespace GameEngine {
	class StringHelpers
	{
	public:

		static std::vector<std::string> Split(std::string s, std::string delimiter) {
			std::vector<std::string> retV = std::vector<std::string>();

			size_t pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				retV.push_back(token);
				s.erase(0, pos + delimiter.length());
			}
			retV.push_back(s);
			return retV;
		}

		static std::string GetExtension(std::string st)
		{
			auto split = Split(st, ".");
			if (split.size() <= 1) return "";
			return split.at(split.size() - 1);
		}

	private:

	};
}