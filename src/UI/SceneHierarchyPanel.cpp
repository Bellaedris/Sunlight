//
// Created by belle on 23/02/2026.
//

#include "SceneHierarchyPanel.h"

#include "imgui/IconsFontAwesome4.h"

namespace sun::ui
{
SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<lum::rdr::SceneDesc>& scene, const std::shared_ptr<EditorState>& editorState)
    : m_scene(scene)
    , m_state(editorState)
{
}

void SceneHierarchyPanel::Render()
{
    ImGui::Begin("Hierarchy");
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 3.f));

        float size = ImGui::GetContentRegionAvail().x;
        if (ImGui::Button(ICON_FA_PLUS " New node", {size, .0f}))
        {
            m_state->temp.m_selectedNode = m_scene->AddNode();
        }

        lum::Node3D* node  = m_scene->RootNode();
        shouldMoveNode = {};
        shouldRemoveNode = {};

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth |
                                   ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_FramePadding |
                                   ImGuiTreeNodeFlags_AllowOverlap;
        RenderNode(node, flags);
        if (shouldMoveNode)
        {
            lum::Node3D* src = shouldMoveNode->source;
            lum::Node3D* dst = shouldMoveNode->destination;
            //we can't reparent a node to one of its child
            if (dst->HasAncestor(src) == false && dst != src->Parent())
                src->Parent()->TransferChild(src, dst);
        }

        if (shouldRemoveNode)
        {
            shouldRemoveNode->source->RemoveChild(shouldRemoveNode->destination);
            m_state->temp.m_selectedNode = nullptr;
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void SceneHierarchyPanel::RenderNode(lum::Node3D* node, ImGuiTreeNodeFlags flags)
{
    // recurse into each child
    for (auto&& child : node->Children())
    {
        int localFlags = flags;
        uuids::uuid nodeId = child->UUID();
        if (m_state->temp.m_selectedNode && nodeId == m_state->temp.m_selectedNode->UUID())
            localFlags |= ImGuiTreeNodeFlags_Selected;

        if (child->HasChild() == false)
        {
            localFlags |= ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;

            ImGui::PushID(to_string(nodeId).c_str());
            ImGui::TreeNodeEx(child->Name().c_str(), localFlags);
            if (ImGui::IsItemClicked())
            {
                // reset focus so any ongoing change in the inspector does not get incorrectly bound to the new selected
                //ImGui::SetWindowFocus(nullptr);
                m_state->temp.m_selectedNode = child.get();
            }
            if (shouldMoveNode.has_value() == false)
                shouldMoveNode = HandleDragDrop(child);

            float elementHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - elementHeight);
            if (ImGui::Button(ICON_FA_TRASH))
            {
                ImGui::OpenPopup("Delete node");
            }
            if (shouldRemoveNode.has_value() == false)
                shouldRemoveNode = DeleteNodeModal(node, child.get());
            ImGui::PopID();
        }
        else
        {
            ImGui::PushID(to_string(nodeId).c_str());
            bool nodeOpen = ImGui::TreeNodeEx(child->Name().c_str(), localFlags);
            if (ImGui::IsItemClicked())
            {
                //ImGui::SetWindowFocus(nullptr);
                m_state->temp.m_selectedNode = child.get();
            }
            if (shouldMoveNode.has_value() == false)
                shouldMoveNode = HandleDragDrop(child);
            float elementHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - elementHeight);
            if (ImGui::Button(ICON_FA_TRASH))
            {
                ImGui::OpenPopup("Delete node");
            }
            if (shouldRemoveNode.has_value() == false)
                shouldRemoveNode = DeleteNodeModal(node, child.get());
            if (nodeOpen)
            {
                RenderNode(child.get(), flags);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }
}

std::optional<SceneHierarchyPanel::DelayedNodeAction> SceneHierarchyPanel::HandleDragDrop(const std::unique_ptr<lum::Node3D> &node)
{
    if (ImGui::BeginDragDropSource())
    {
        lum::Node3D* ptr = node.get();
        ImGui::SetDragDropPayload("NODE3D", &ptr, sizeof(lum::Node3D*));
        ImGui::Text("%s", node->Name().c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE3D"))
        {
            lum::Node3D* payloadNode = *static_cast<lum::Node3D* const*> (payload->Data);
            // we should wait for the next frame since we just invalidated the iterator
            ImGui::EndDragDropTarget();
            return{DelayedNodeAction(payloadNode, node.get())};
        }
        ImGui::EndDragDropTarget();
    }

    return {};
}

std::optional<SceneHierarchyPanel::DelayedNodeAction> SceneHierarchyPanel::DeleteNodeModal(lum::Node3D* parent, lum::Node3D* node)
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Delete node", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("This action will delete the Node: %s\n" \
                    "Until I add a ctrl+z feature, this cannot be undone!", node->Name().c_str());
        ImGui::Separator();

        if (ImGui::Button("Delete", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            return {DelayedNodeAction(parent, node)};
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
    return {};
}
} // sun::ui