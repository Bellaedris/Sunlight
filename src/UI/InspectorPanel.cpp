//
// Created by belle on 24/02/2026.
//

#include "InspectorPanel.h"

#include "Lumiere/Components/MeshRenderer.h"
#include "imgui/IconsFontAwesome4.h"
#include "imgui/imgui_internal.h"

namespace sun::ui
{
InspectorPanel::InspectorPanel(const std::shared_ptr<EditorState> &state)
    : m_state(state)
{
    m_fileBrowser.SetTitle("Mesh selection");
    m_fileBrowser.SetTypeFilters({".gltf", ".glb", ".obj", ".fbx"});
}

void InspectorPanel::Render()
{
    ImGui::Begin("Inspector");
    {
        if (m_state->temp.m_selectedNode != nullptr)
        {
            lum::Node3D* node = m_state->temp.m_selectedNode;
            
            ImGui::InputText("Name", m_state->temp.m_selectedNode->Name().data(), lum::Node3D::NODE_NAME_MAX_LENGTH);

            // component sub region flags
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen;
            // Transform
            DrawTransformInspector(m_state->temp.m_selectedNode->GetTransform(), flags);

            // Mesh renderer
            if (auto opt = node->GetComponent<lum::comp::MeshRenderer>())
            {
                lum::comp::MeshRenderer* renderer = opt.value();
                if (ImGui::TreeNodeEx(ICON_FA_CUBE " Mesh Renderer", flags))
                {
                    if (renderer->Mesh() != nullptr)
                        ImGui::Text("Current Mesh: %s", renderer->Mesh()->Name().c_str());
                    if (ImGui::Button("Pick a mesh"))
                    {
                        // load a mesh from file/resourcesManager
                        m_fileBrowser.Open();
                    }
                    ImGui::TreePop();
                }

                m_fileBrowser.Display();
                if (m_fileBrowser.HasSelected())
                {
                    renderer->SetMesh(m_fileBrowser.GetSelected().string());
                    m_fileBrowser.ClearSelected();
                }
            }

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
                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();
}

void InspectorPanel::DrawTransformInspector(lum::comp::Transform& transform, ImGuiTreeNodeFlags flags)
{
    if (ImGui::TreeNodeEx(ICON_FA_ARROWS " Transform", flags))
    {
        ImGui::BeginTable("Transform", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX);

        ImGui::TableNextRow();
        TransformSlider("Position", transform.LocalPosition(), .0f, [&transform](const glm::vec3& vector)
        {
            transform.SetLocalPosition(vector);
        });

        ImGui::TableNextRow();
        TransformSlider("Rotation", transform.LocalRotation(), .0f, [&transform](const glm::vec3& vector)
        {
            transform.SetLocalRotation(vector);
        });

        ImGui::TableNextRow();
        TransformSlider("Scale", transform.LocalScale(), 1.f, [&transform](const glm::vec3& vector)
        {
            transform.SetLocalScale(vector);
        });
        ImGui::EndTable();
        ImGui::TreePop();
    }
}

void InspectorPanel::TransformSlider(const char* name, glm::vec3 vector, float defaultValue, std::function<void(const glm::vec3&)> updateVector)
{
    ImGui::AlignTextToFramePadding();

    ImGui::TableNextColumn();
	ImGui::Text("%s", name);

    ImGui::TableNextColumn();
	ImGui::PushMultiItemsWidths(3, ImGui::GetContentRegionAvail().x - 30);

	ImGui::PushID(name);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	//ImGui::PushStyleColor(ImGuiCol_ButtonActive, EditorCol_Secondary2);
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
		{
			if (ImGui::Button("X")) {
				updateVector(glm::vec3(defaultValue, vector.y, vector.z));
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##X", &vector.x, 0.5f))
			{
			    updateVector(vector);
			}
			ImGui::SameLine();
		}
		ImGui::PopStyleColor(2); // Button, ButtonHovered
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 1, 0, 1));
		{
			if (ImGui::Button("Y")) {
				updateVector(glm::vec3(vector.x, defaultValue, vector.z));
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##Y", &vector.y, 0.5f))
			{
			    updateVector(vector);
			}
			ImGui::SameLine();
		}
		ImGui::PopStyleColor(2); // Button, ButtonHovered
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 1, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 1, 1));
		{
			if (ImGui::Button("Z"))
			{
				updateVector(glm::vec3(vector.x, vector.y, defaultValue));
			}
			ImGui::SameLine();
			if (ImGui::DragFloat("##Z", &vector.z, 0.5f))
			{
			    updateVector(vector);
			}
		}
		ImGui::PopStyleColor(2); // Button, ButtonHovered
	}

	ImGui::PopStyleVar();
	ImGui::PopID();
}
} // sun::ui