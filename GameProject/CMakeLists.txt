# CMakeList.txt: CMake-Projekt für "GameProject". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.20)

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (GameProject "GameProject.cpp" "GameProject.h")


include(CMakeSources.cmake)


configure_file(GameProjectConfig.h.in GameProjectConfig.h)

target_include_directories (GameProject
PUBLIC
"${PROJECT_BINARY_DIR}"
)


target_link_libraries( GameProject
	PUBLIC
	libglew_static
	EnTT
	spdlog
	glfw
	glm
)
