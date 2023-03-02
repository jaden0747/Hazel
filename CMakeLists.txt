cmake_minimum_required(VERSION 3.0)
project(hazel-app)

add_subdirectory(Hazel/vendor/spdlog)

# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "/MD")
# set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
# set(CMAKE_CXX_FLAGS_RELEASE "/MD")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Hazel/vendor/GLFW)
add_subdirectory(Hazel/vendor/Glad)

add_library(MyImGui STATIC 
    Hazel/vendor/imgui/imconfig.h
    Hazel/vendor/imgui/imgui.h
    Hazel/vendor/imgui/imgui.cpp
    Hazel/vendor/imgui/imgui_draw.cpp
    Hazel/vendor/imgui/imgui_internal.h
    Hazel/vendor/imgui/imgui_tables.cpp
    Hazel/vendor/imgui/imgui_widgets.cpp
    Hazel/vendor/imgui/imstb_rectpack.h
    Hazel/vendor/imgui/imstb_textedit.h
    Hazel/vendor/imgui/imstb_truetype.h
    Hazel/vendor/imgui/imgui_demo.cpp
)

target_include_directories(MyImGui PUBLIC Hazel/vendor/imgui/)


find_package(OpenGL REQUIRED)

add_library(hazel SHARED 
    Hazel/src/hzpch.cpp
    Hazel/src/Hazel/Application.cpp
    Hazel/src/Hazel/Application.h
    Hazel/src/Hazel/Log.cpp
    Hazel/src/Hazel/Log.h
    Hazel/src/Hazel/Core.h
    Hazel/src/Hazel/EntryPoint.h
    Hazel/src/Hazel/Events/ApplicationEvent.h
    Hazel/src/Hazel/Events/Event.h
    Hazel/src/Hazel/Events/KeyEvent.h
    Hazel/src/Hazel/Events/MouseEvent.h
    Hazel/src/Hazel/Window.h
    Hazel/src/Hazel/Layer.cpp
    Hazel/src/Hazel/Layer.h
    Hazel/src/Hazel/LayerStack.cpp
    Hazel/src/Hazel/LayerStack.h
    Hazel/src/Hazel/ImGui/ImGuiLayer.cpp
    Hazel/src/Hazel/ImGui/ImGuiLayer.h
    Hazel/src/Platform/Windows/WindowsWindow.h
    Hazel/src/Platform/Windows/WindowsWindow.cpp
    Hazel/src/Platform/OpenGL/ImGuiOpenGLRenderer.cpp
    Hazel/src/Platform/OpenGL/ImGuiOpenGLRenderer.h
)

target_include_directories(hazel PUBLIC 
    ${PROJECT_SOURCE_DIR}/Hazel/
    ${PROJECT_SOURCE_DIR}/Hazel/src/
    ${PROJECT_SOURCE_DIR}/Hazel/vendor/GLFW/include
    ${PROJECT_SOURCE_DIR}/Hazel/vendor/Glad/include
)

target_compile_definitions(hazel PRIVATE 
    HZ_PLATFORM_WINDOWS 
    HZ_BUILD_DLL
    GLFW_INCLUDE_NONE
)

target_precompile_headers(hazel PUBLIC Hazel/src/hzpch.h)

target_link_libraries(hazel PUBLIC spdlog glfw OpenGL::GL Dwmapi Glad MyImGui)

add_executable(hazel-app  
    Sandbox/src/SandboxApp.cpp
    Hazel/Hazel.h
)

target_include_directories(hazel-app PUBLIC
    ${PROJECT_SOURCE_DIR}/Hazel/
    ${PROJECT_SOURCE_DIR}/Hazel/src/
)

target_compile_definitions(hazel-app PRIVATE 
    HZ_PLATFORM_WINDOWS 
)

target_link_libraries(hazel-app PRIVATE hazel)