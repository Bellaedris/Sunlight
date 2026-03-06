//
// Created by belle on 24/02/2026.
//

#include "InspectorPanel.h"

#include "Lumiere/Components/Light.h"
#include "Lumiere/Components/MeshRenderer.h"
#include "Lumiere/Components/Script.h"
#include "imgui/IconsFontAwesome4.h"
#include "imgui/imgui_internal.h"

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

            // Transform
            node->GetTransform().DrawInspector();

            // Components
            for (auto&& component : node->Components())
                component->DrawInspector();

            // new component creation
            float buttonWidth = ImGui::GetContentRegionAvail().x * (2.f / 3.f);
            float size = ImGui::GetStyle().FramePadding.x * 2.0f;
            ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - buttonWidth) * .5f);
            if (ImGui::Button("AddComponent", {buttonWidth, 0}))
            {
                ImGui::OpenPopup("ComponentSelection");
            }

            if (ImGui::IsPopupOpen("ComponentSelection"))
            {
                ImVec2 addButtonPos = ImGui::GetItemRectMin();
                ImVec2 addButtonSize = ImGui::GetItemRectSize();
                ImGui::SetNextWindowSizeConstraints(
                    ImVec2(.0f, .0f),
                    ImVec2(FLT_MAX, 300.0f) // if the popup contains too many components, adds a scrollbar
                );
                ImGui::SetNextWindowPos(ImVec2(addButtonPos.x, addButtonPos.y + addButtonSize.y));
                ImGui::SetNextWindowSize(ImVec2(buttonWidth, 0.0f));
            }

            if (ImGui::BeginPopup("ComponentSelection"))
            {
                ComponentCreationButton<lum::comp::MeshRenderer>(node, "Mesh Renderer", ICON_FA_CUBE);
                ComponentCreationButton<lum::comp::Script>(node, "Lua Script", ICON_FA_FILE_CODE_O);
                ComponentCreationButton<lum::comp::Light>(node, "Light Source", ICON_FA_LIGHTBULB_O);
                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();
}
} // sun::ui