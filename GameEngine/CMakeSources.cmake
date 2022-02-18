set(SOURCES ${SOURCES}
		Engine/Engine.hpp
		Engine/Scene/Scene.hpp
		Engine/Entities/Entity.hpp
		Engine/Entities/data/NativeScript.hpp

		Engine/Entities/BaseComponents.hpp

		Engine/Renderer/Renderer.hpp
		Engine/Core/Window.hpp
		Engine/Utils/Profiling/ProfileInstance.hpp
		Engine/Utils/Profiling/ProcessTimeProfile.hpp
		Engine/Entities/data/DataStructs.hpp
		Engine/ResourceManagement/TextureResourceManager.hpp
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

		Engine/Renderer/ImGUI/ImGUIStyling.hpp

		Engine/Utils/stb/stb_image.hpp


		Engine/Renderer/Shaders/ShaderProgram.hpp
		Engine/Renderer/Renderable.hpp

		Engine/Editor/EditorView.hpp

		

)

set(SOURCES ${SOURCES}
		Engine/Engine.cpp

		Engine/Entities/Entity.cpp
		Engine/Scene/Scene.cpp

		Engine/Renderer/Renderer.cpp
		Engine/Core/Window.cpp

		Engine/Utils/Profiling/ProfileInstance.cpp
		Engine/Utils/Profiling/ProcessTimeProfile.cpp
		Engine/ResourceManagement/TextureResourceManager.cpp
		Engine/Utils/Serialisation/TextureLoader.cpp



		Engine/Renderer/ImGUI/imgui/imgui.cpp
		Engine/Renderer/ImGUI/imgui/imgui_demo.cpp
		Engine/Renderer/ImGUI/imgui/imgui_draw.cpp
		Engine/Renderer/ImGUI/imgui/imgui_tables.cpp
		Engine/Renderer/ImGUI/imgui/imgui_widgets.cpp
		Engine/Renderer/ImGUI/imgui/backends/imgui_impl_glfw.cpp
		Engine/Renderer/ImGUI/imgui/backends/imgui_impl_opengl3.cpp

		Engine/Renderer/Shaders/ShaderProgram.cpp
		Engine/Renderer/Renderable.cpp
		Engine/Editor/EditorView.cpp


		Engine/Entities/data/NativeScript.cpp
)