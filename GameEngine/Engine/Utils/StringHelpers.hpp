#pragma once
#include <string>
#include <vector>
namespace GameEngine {
	class StringHelpers
	{
	public:

		static std::vector<std::string> Split(std::string string, std::string spliter) {
			std::vector<std::string> retV = std::vector<std::string>();

			int start = 0;
			int end = string.find(spliter);
			while (end != -1) {
				retV.push_back(string.substr(start, end - start));
				start = end + spliter.size();
				end = string.find(spliter, start);
			}

			return retV;
		}

	private:

	};
}