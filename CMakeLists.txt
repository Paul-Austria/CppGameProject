cmake_minimum_required (VERSION 3.20)

project ("GameProject" VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory ("GameProject")
add_subdirectory("Thirdparty")

file(INSTALL "Resources" DESTINATION "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}")
