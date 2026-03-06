//
// Created by belle on 24/02/2026.
//

#pragma once
#include "IEditorPanel.h"
#include "../EditorState.h"
#include "Lumiere/Components/MeshRenderer.h"
#include "imgui/imfilebrowser.h"

namespace sun::ui
{
class InspectorPanel : public IEditorPanel
{
private:
    std::shared_ptr<EditorState> m_state;
    int m_selectedComponentType {0};

    template <typename T>
    void ComponentCreationButton(lum::Node3D* node, const char* name, const char* icon);

public:
    InspectorPanel(const std::shared_ptr<EditorState>& state);

    void Render() override;
};

template<typename T>
void InspectorPanel::ComponentCreationButton(lum::Node3D* node, const char *name, const char* icon)
{
    if (node->HasComponent<T>() == false)
        if (ImGui::Selectable((icon + std::string(" ") + name).c_str(), false))
            node->AddComponent<T>();
}
} // sun::ui