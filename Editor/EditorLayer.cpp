#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hazel
{

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
    , m_cameraController(1920.0f / 1080.0f, true)
    , m_squareColor({0.2f, 0.3f, 0.8f, 1.0f})
{
    int a = 0;
}

void EditorLayer::onAttach()
{
    HZ_PROFILE_FUNCTION();
    m_checkerboardTexture = Texture2D::create("resources/assets/textures/Checkerboard.png");

    FramebufferSpecification fbSpec;
    fbSpec.width  = 1920;
    fbSpec.height = 1080;
    m_framebuffer = Framebuffer::create(fbSpec);

    m_activeScene = createRef<Scene>();

    auto square = m_activeScene->createEntity("Green Square");
    square.addComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    m_squareEntity = square;

    m_cameraEntity = m_activeScene->createEntity("Camera Entity");
    m_cameraEntity.addComponent<CameraComponent>();

    m_secondCamera = m_activeScene->createEntity("Clip-Space Entity");
    auto& cc       = m_secondCamera.addComponent<CameraComponent>();
    cc.m_primary   = false;

    class CameraController : public ScriptableEntity
    {
    public:
        void onCreate()
        {
            auto& transform = getComponent<TransformComponent>().m_transform;
            transform[3][0] = rand() % 10 - 5.0f;
        }

        void onDestroy()
        {
        }

        void onUpdate(Timestep ts)
        {
            auto& transform = getComponent<TransformComponent>().m_transform;
            float speed     = 5.0f;

            if (Input::isKeyPressed(Key::A))
            {
                transform[3][0] -= speed * ts;
            }
            if (Input::isKeyPressed(Key::D))
            {
                transform[3][0] += speed * ts;
            }
            if (Input::isKeyPressed(Key::W))
            {
                transform[3][1] += speed * ts;
            }
            if (Input::isKeyPressed(Key::S))
            {
                transform[3][1] -= speed * ts;
            }
        }
    };

    m_cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
    m_secondCamera.addComponent<NativeScriptComponent>().bind<CameraController>();

    m_sceneHierarchyPanel.setContext(m_activeScene);
}

void EditorLayer::onDetach()
{
    HZ_PROFILE_FUNCTION();
}

void EditorLayer::onUpdate(Timestep ts)
{
    HZ_PROFILE_FUNCTION();

    // resize
    if (FramebufferSpecification spec = m_framebuffer->getSpecification();
        m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
        (spec.width != m_viewportSize.x || spec.height != m_viewportSize.y))
    {
        m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
        m_cameraController.onResize(m_viewportSize.x, m_viewportSize.y);

        m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
    }

    // update
    if (m_viewportFocused)
        m_cameraController.onUpdate(ts);

    // render
    Renderer2D::resetStats();
    m_framebuffer->bind();
    RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::clear();

    m_activeScene->onUpdate(ts);

    m_framebuffer->unbind();
}

void EditorLayer::onImGuiRender()
{
    HZ_PROFILE_FUNCTION();

    static bool               dockspaceOpen             = true;
    static bool               opt_fullscreen_persistant = true;
    bool                      opt_fullscreen            = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags           = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Dockspace Demo", &dockspaceOpen, window_flags);
    {
        ImGui::PopStyleVar();
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Dockspace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    Application::get().close();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_sceneHierarchyPanel.onImGuiRender();

        ImGui::Begin("Settings");
        {
            auto stats = Renderer2D::getStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.drawCalls);
            ImGui::Text("Quads: %d", stats.quadCount);
            ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
            ImGui::Text("Indices: %d", stats.getTotalIndexCount());

            // auto& squareColor = m_activeScene->reg().get<SpriteRendererComponent>(m_squareEntity).m_color;
            // ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
            if (m_squareEntity)
            {
                ImGui::Separator();
                auto& tag = m_squareEntity.getComponent<TagComponent>().m_tag;
                ImGui::Text("%s", tag.c_str());
                auto& squareColor = m_squareEntity.getComponent<SpriteRendererComponent>().m_color;
                ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
                ImGui::Separator();
            }

            ImGui::DragFloat3(
                "Camera Transform", glm::value_ptr(m_cameraEntity.getComponent<TransformComponent>().m_transform[3]));

            if (ImGui::Checkbox("Camera A", &m_primaryCamera))
            {
                m_cameraEntity.getComponent<CameraComponent>().m_primary = m_primaryCamera;
                m_secondCamera.getComponent<CameraComponent>().m_primary = !m_primaryCamera;
            }

            {
                auto& camera    = m_secondCamera.getComponent<CameraComponent>().m_camera;
                float orthoSize = camera.getOrthographicSize();
                if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
                {
                    camera.setOrthographicSize(orthoSize);
                }
            }
        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 0.0f});
        ImGui::Begin("Viewport");
        {
            m_viewportFocused = ImGui::IsWindowFocused();
            m_viewportHovered = ImGui::IsWindowHovered();
            Application::get().getImguiLayer()->blockEvents(!m_viewportFocused || !m_viewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_viewportSize           = {viewportPanelSize.x, viewportPanelSize.y};
            uint32_t textureID       = m_framebuffer->getColorAttachmentRendererID();
            ImGui::Image(
                (ImTextureID*)textureID, ImVec2{m_viewportSize.x, m_viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void EditorLayer::onEvent(Event& e)
{
    m_cameraController.onEvent(e);
}

} // namespace hazel
