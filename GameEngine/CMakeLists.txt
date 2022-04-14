project (GameEngine VERSION 0.0.1)

include(CMakeSources.cmake)
add_library(GameEngine SHARED ${SOURCES})

target_compile_definitions(
GameEngine
PUBLIC

$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:DEBUG>
)




target_include_directories (GameEngine
PUBLIC
"${CMAKE_CURRENT_BINARY_DIR}"
"${CMAKE_CURRENT_SOURCE_DIR}"
"${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
)


target_link_libraries(GameEngine
	PUBLIC
	libglew_static
	EnTT
	lua_static
	sol2
	spdlog
	glfw
	glm
	"${LUA_LIBARIES}"

)


#install(TARGETS GameEngine
#    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
#    )


configure_file(GameProjectConfig.h.in GameProjectConfig.h)
