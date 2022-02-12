cmake_minimum_required (VERSION 3.20)


file(GLOB_RECURSE DEMO_HEADER_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.h" "*.hpp" "*.inl"
		)
file(GLOB_RECURSE DEMO_SOURCE_FILES
		RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
		"*.c" "*.cpp" "*.rc"
		)

add_executable (RunTime "GameProject.cpp" "GameProject.h"  "ExampleScene.hpp" "ExampleScene.cpp")

target_compile_features(RunTime PUBLIC cxx_std_20)
target_include_directories(RunTime PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)



target_link_libraries(RunTime GameEngine)

