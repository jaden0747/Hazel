#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include "Hazel/Scene/Components.h"
#include "hzpch.h"

namespace hazel
{

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
{
    setContext(context);
}

void SceneHierarchyPanel::setContext(const Ref<Scene>& context)
{
    m_context = context;
}

void SceneHierarchyPanel::onImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    for (auto entityID : m_context->m_registry.view<entt::entity>())
    {
        Entity entity{entityID, m_context.get()};
        drawEntityNode(entity);
    }

    ImGui::End();
}

void SceneHierarchyPanel::drawEntityNode(Entity entity)
{
    auto& tag = entity.getComponent<TagComponent>().m_tag;

    ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

    if (ImGui::IsItemClicked())
    {
        m_selectionContext = entity;
    }

    if (opened)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
        if (opened)
        {
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}

}  // namespace hazel
