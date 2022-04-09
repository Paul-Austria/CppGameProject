#pragma once
#include <string>
#include <filesystem>

namespace GameEngine {
	class EditorView;
	class ContentBrowser
	{
	public:
		~ContentBrowser();
		ContentBrowser() = default;
		void Render(EditorView* editor);
	private:
		std::string rootPath;
		std::string currentSubPath;


		void HandleFileClick(std::string fileName,EditorView* editor);
	};
}