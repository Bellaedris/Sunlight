//
// Created by belle on 23/02/2026.
//

#include "SceneHierarchyPanel.h"

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
        lum::Node3D& node = m_scene->RootNode();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesFull;
        RenderNode(node, flags);
    }
    ImGui::End();
}

void SceneHierarchyPanel::RenderNode(const lum::Node3D& node, ImGuiTreeNodeFlags flags)
{
    // recurse into each child
    std::optional<DelayedDragDrop> shouldMoveNode;
    for (auto&& child : node.Children())
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
                ImGui::SetWindowFocus(nullptr);
                m_state->temp.m_selectedNode = child.get();
            }
            if (shouldMoveNode.has_value() == false)
                shouldMoveNode = HandleDragDrop(child);
            ImGui::PopID();
        }
        else
        {
            ImGui::PushID(to_string(nodeId).c_str());
            bool nodeOpen = ImGui::TreeNodeEx(child->Name().c_str(), localFlags);
            if (ImGui::IsItemClicked())
            {
                ImGui::SetWindowFocus(nullptr);
                m_state->temp.m_selectedNode = child.get();
            }
            if (shouldMoveNode.has_value() == false)
                shouldMoveNode = HandleDragDrop(child);
            if (nodeOpen)
            {
                RenderNode(*child, flags);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }
    if (shouldMoveNode)
    {
        lum::Node3D* src = shouldMoveNode->source;
        lum::Node3D* dst = shouldMoveNode->destination;
        //we can't reparent a node to one of its child
        if (dst->HasAncestor(src) == false && src->HasAncestor(dst) == false)
            src->Parent()->TransferChild(src, dst);
    }
}

std::optional<SceneHierarchyPanel::DelayedDragDrop> SceneHierarchyPanel::HandleDragDrop(const std::unique_ptr<lum::Node3D> &node)
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
            return{DelayedDragDrop(payloadNode, node.get())};
        }
        ImGui::EndDragDropTarget();
    }

    return {};
}
} // sun::ui