cmake_minimum_required(VERSION 3.19)
project(hazel-app)

add_subdirectory(Hazel/vendor/spdlog)

# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "/MD")
# set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
# set(CMAKE_CXX_FLAGS_RELEASE "/MD")

set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Hazel/vendor/GLFW)
add_subdirectory(Hazel/vendor/Glad)
add_subdirectory(Hazel/vendor/glm)

add_library(MyImGui STATIC 
    Hazel/vendor/imgui/imconfig.h
    Hazel/vendor/imgui/imgui.h
    Hazel/vendor/imgui/imgui.cpp
    Hazel/vendor/imgui/imgui_draw.cpp
    Hazel/vendor/imgui/imgui_internal.h
    # Hazel/vendor/imgui/imgui_tables.cpp
    Hazel/vendor/imgui/imgui_widgets.cpp
    Hazel/vendor/imgui/imstb_rectpack.h
    Hazel/vendor/imgui/imstb_textedit.h
    Hazel/vendor/imgui/imstb_truetype.h
    Hazel/vendor/imgui/imgui_demo.cpp
)

target_include_directories(MyImGui PUBLIC Hazel/vendor/imgui)

# add_library(stb_image STATIC
#     Hazel/vendor/stb_image/stb_image.h
#     Hazel/vendor/stb_image/stb_image.cpp
# )

# target_include_directories(stb_image PUBLIC Hazel/src  Hazel/vendor/stb_image)

find_package(OpenGL REQUIRED)

add_library(hazel STATIC 
    Hazel/src/hzpch.cpp
    Hazel/src/Hazel/Core/Input.cpp
    Hazel/src/Hazel/Core/Application.cpp
    Hazel/src/Hazel/Core/Log.cpp
    Hazel/src/Hazel/Core/Layer.cpp
    Hazel/src/Hazel/Core/LayerStack.cpp
    Hazel/src/Hazel/Renderer/OrthographicCameraController.cpp
    Hazel/src/Hazel/ImGui/ImGuiLayer.cpp
    Hazel/src/Hazel/ImGui/ImGuiBuild.cpp
    Hazel/src/Hazel/Renderer/Shader.cpp
    Hazel/src/Hazel/Renderer/Buffer.cpp
    Hazel/src/Hazel/Renderer/Renderer.cpp
    Hazel/src/Hazel/Renderer/Renderer2D.cpp
    Hazel/src/Hazel/Renderer/VertexArray.cpp
    Hazel/src/Hazel/Renderer/RendererAPI.cpp
    Hazel/src/Hazel/Renderer/RenderCommand.cpp
    Hazel/src/Hazel/Renderer/RendererAPI.cpp
    Hazel/src/Hazel/Renderer/OrthographicCamera.cpp
    Hazel/src/Hazel/Renderer/Texture.cpp
    Hazel/src/Platform/Windows/WindowsWindow.cpp
    Hazel/src/Platform/Windows/WindowsInput.cpp
    Hazel/src/Platform/OpenGL/OpenGLContext.cpp
    Hazel/src/Platform/OpenGL/OpenGLBuffer.cpp
    Hazel/src/Platform/OpenGL/OpenGLVertexArray.cpp
    Hazel/src/Platform/OpenGL/OpenGLRendererAPI.cpp
    Hazel/src/Platform/OpenGL/OpenGLShader.cpp
    Hazel/src/Platform/OpenGL/OpenGLTexture.cpp
    Hazel/vendor/stb_image/stb_image.h
    Hazel/vendor/stb_image/stb_image.cpp
)

target_include_directories(hazel PUBLIC 
    ${PROJECT_SOURCE_DIR}/Hazel/
    ${PROJECT_SOURCE_DIR}/Hazel/src/
    ${PROJECT_SOURCE_DIR}/Hazel/src/Hazel
    ${PROJECT_SOURCE_DIR}/Hazel/vendor/GLFW/include
    ${PROJECT_SOURCE_DIR}/Hazel/vendor/Glad/include
    ${PROJECT_SOURCE_DIR}/Hazel/vendor/glm/
    ${PROJECT_SOURCE_DIR}/Hazel/vendor/stb_image/
    ${PROJECT_SOURCE_DIR}/Hazel/vendor/imgui/
)

target_compile_definitions(hazel PRIVATE 
    HZ_BUILD_DLL
    GLFW_INCLUDE_NONE
    _CRT_SECURE_NO_WARNINGS
)

target_precompile_headers(hazel PUBLIC Hazel/src/hzpch.h)

target_link_libraries(hazel PUBLIC spdlog glfw OpenGL::GL Dwmapi Glad MyImGui glm)

add_executable(hazel-app  
    Sandbox/src/SandboxApp.cpp
    Sandbox/src/Sandbox2D.cpp
    Hazel/Hazel.h
)

target_include_directories(hazel-app PUBLIC
    ${PROJECT_SOURCE_DIR}/Hazel/
    ${PROJECT_SOURCE_DIR}/Hazel/src/
    ${PROJECT_SOURCE_DIR}/Hazel/vendor/
)

target_compile_definitions(hazel-app PRIVATE 
    _CRT_SECURE_NO_WARNINGS
)

target_link_libraries(hazel-app PRIVATE hazel)