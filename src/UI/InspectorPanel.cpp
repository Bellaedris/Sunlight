//
// Created by belle on 24/02/2026.
//

#include "InspectorPanel.h"

#include "Lumiere/Components/MeshRenderer.h"

namespace sun::ui
{
InspectorPanel::InspectorPanel(const std::shared_ptr<EditorState> &state)
    : m_state(state)
{

}

void InspectorPanel::Render()
{
    ImGui::Begin("Inspector");
    {
        if (m_state->temp.m_selectedNode != nullptr)
        {
            lum::Node3D* node = m_state->temp.m_selectedNode;
            
            ImGui::InputText("Name", m_state->temp.m_selectedNode->Name().data(), lum::Node3D::NODE_NAME_MAX_LENGTH);
            ImGui::Combo("ComponentType", &m_selectedComponentType, COMPONENT_TYPES.data(), COMPONENT_TYPES.size());
            ImGui::SameLine();
            if (ImGui::Button("Create Component"))
            {
                switch (m_selectedComponentType)
                {
                    case 0:
                        node->AddComponent<lum::comp::MeshRenderer>();
                    default:
                        break;
                }
            }

            m_state->temp.m_selectedNode->GetTransform().DrawInspector();
            for (auto&& component : node->Components())
            {
                component->DrawInspector();
            }
        }
    }
    ImGui::End();
}
} // sun::ui