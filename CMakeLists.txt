cmake_minimum_required (VERSION 3.20)

project (GameEngine VERSION 0.0.1)


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON CACHE BOOL "Export all symbols")





set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory ("GameEngine")
add_subdirectory("Thirdparty")
add_subdirectory("RunTime")



message(${PROJECT_BINARY_DIR})


