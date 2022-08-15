cmake_minimum_required(VERSION 3.0)
project(hazel-app)

add_subdirectory(Hazel/vendor/spdlog)

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
)

target_include_directories(hazel PUBLIC 
    ${PROJECT_SOURCE_DIR}/Hazel/
    ${PROJECT_SOURCE_DIR}/Hazel/src/
)

target_compile_definitions(hazel PRIVATE 
    HZ_PLATFORM_WINDOWS 
    HZ_BUILD_DLL
)

target_precompile_headers(hazel PUBLIC Hazel/src/hzpch.h)

target_link_libraries(hazel PUBLIC spdlog)

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