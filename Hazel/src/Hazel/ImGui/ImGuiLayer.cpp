#include "Hazel/ImGui/ImGuiLayer.h"

// clang-format off
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
// clang-format on

#include "Hazel/Core/Application.h"
#include "hzpch.h"

// TEMP
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace hazel
{
ImGuiLayer::ImGuiLayer(const std::string& iniFilename)
    : Layer("ImGuiLayer")
    , m_iniFilename(iniFilename)
{
}

void ImGuiLayer::onAttach()
{
    HZ_PROFILE_FUNCTION();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = m_iniFilename.c_str();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    ImGui::StyleColorsDark();

    ImFont* customFont = io.Fonts->AddFontFromFileTTF("Hazel/vendor/imgui/misc/fonts/DroidSans.ttf", 26.0f);

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Application& app    = Application::get();
    GLFWwindow*  window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::onDetach()
{
    HZ_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::begin()
{
    HZ_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::onEvent(Event& e)
{
    if (m_blockEvents)
    {
        ImGuiIO& io = ImGui::GetIO();
        e.m_handled |= e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        e.m_handled |= e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
}

void ImGuiLayer::end()
{
    HZ_PROFILE_FUNCTION();

    ImGuiIO&     io  = ImGui::GetIO();
    Application& app = Application::get();
    io.DisplaySize   = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

} // namespace hazel
