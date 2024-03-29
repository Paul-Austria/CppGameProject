set(SOURCES ${SOURCES}

		Engine/Utils/IO/Input.hpp

		Engine/sol/config.hpp
		Engine/sol/forward.hpp
		Engine/sol/assert.hpp

		Engine/Entities/data/LuaScriptHandler.hpp
		Engine/Entities/data/LuaScript.hpp
		Engine/Engine.hpp

		Engine/Core/Window.hpp
		Engine/Scene/Scene.hpp
		Engine/Scene/ProjectData.hpp


		Engine/Entities/Entity.hpp
		Engine/Entities/data/NativeScript.hpp

		Engine/Entities/BaseComponents.hpp

		Engine/Renderer/Renderer.hpp
		Engine/Entities/data/DataStructs.hpp
		Engine/Utils/Serialisation/json/json.hpp
		Engine/Utils/Serialisation/TextureLoader.hpp



		Engine/Renderer/ImGUI/imgui/imconfig.h
		Engine/Renderer/ImGUI/imgui/imgui.h
		Engine/Renderer/ImGUI/imgui/imgui_internal.h
		Engine/Renderer/ImGUI/imgui/backends/imgui_impl_glfw.h
		Engine/Renderer/ImGUI/imgui/backends/imgui_impl_opengl3.h
		Engine/Renderer/ImGUI/imgui/imgui_internal.h
		Engine/Renderer/ImGUI/imgui/imstb_rectpack.h
		Engine/Renderer/ImGUI/imgui/imstb_textedit.h
		Engine/Renderer/ImGUI/imgui/imstb_truetype.h
		
		Engine/Renderer/ImGUI/imgui/dirent/dirent.h
		Engine/Renderer/ImGUI/imgui/ImGuiFileDialog.h
		Engine/Renderer/ImGUI/imgui/ImGuiFileDialogConfig.h

		Engine/Renderer/ImGUI/ImGUIStyling.hpp

		Engine/Utils/stb/stb_image.hpp
		Engine/Utils/Profiling/ProfileInstance.hpp
		Engine/Utils/Profiling/ProcessTimeProfile.hpp
		Engine/Utils/Serialisation/ProjectSerialisation.hpp
		Engine/Renderer/Shaders/ShaderProgram.hpp
		Engine/Renderer/Renderable.hpp

		Engine/Editor/EditorView.hpp
		Engine/Editor/ContentBrowser.hpp

		Engine/ResourceManagement/TextureResourceManager.hpp


		Engine/Utils/StringHelpers.hpp

		

)

set(SOURCES ${SOURCES}
		Engine/Utils/IO/Input.cpp	
		Engine/Entities/data/LuaScriptHandler.cpp

		Engine/Engine.cpp

		Engine/Entities/Entity.cpp
		Engine/Entities/data/NativeScript.cpp

		Engine/Scene/Scene.cpp
		Engine/Scene/ProjectData.cpp

		Engine/Renderer/Renderer.cpp
		Engine/Core/Window.cpp

		Engine/Utils/Profiling/ProfileInstance.cpp
		Engine/Utils/Profiling/ProcessTimeProfile.cpp
		Engine/Utils/Serialisation/TextureLoader.cpp
		Engine/Utils/Serialisation/ProjectSerialisation.cpp


		Engine/ResourceManagement/TextureResourceManager.cpp


		Engine/Renderer/ImGUI/imgui/imgui.cpp
		Engine/Renderer/ImGUI/imgui/imgui_demo.cpp
		Engine/Renderer/ImGUI/imgui/imgui_draw.cpp
		Engine/Renderer/ImGUI/imgui/imgui_tables.cpp
		Engine/Renderer/ImGUI/imgui/imgui_widgets.cpp
		Engine/Renderer/ImGUI/imgui/backends/imgui_impl_glfw.cpp
		Engine/Renderer/ImGUI/imgui/backends/imgui_impl_opengl3.cpp
		Engine/Renderer/ImGUI/imgui/ImGuiFileDialog.cpp

		Engine/Renderer/Shaders/ShaderProgram.cpp
		Engine/Renderer/Renderable.cpp
		
		Engine/Editor/EditorView.cpp
		Engine/Editor/ContentBrowser.cpp


)