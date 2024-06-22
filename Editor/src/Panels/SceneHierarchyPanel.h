#pragma once

#include "Hazel/Core/Base.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Scene/Entity.h"
#include "Hazel/Scene/Scene.h"

namespace hazel
{

class SceneHierarchyPanel
{
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& scene);

    void setContext(const Ref<Scene>& scene);

    void onImGuiRender();

private:
    void drawEntityNode(Entity entity);

private:
    Ref<Scene> m_context;
    Entity m_selectionContext;
};

}  // namespace hazel
