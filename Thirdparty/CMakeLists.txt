cmake_minimum_required (VERSION 3.20)


add_subdirectory("spdlog")
add_subdirectory("entt")
add_subdirectory("glfw")
add_subdirectory("glm")
add_subdirectory("--lua-cmake")
add_definitions(-DGLEW_STATIC)
add_subdirectory("glew-cmake")

