//
// Created by belle on 23/02/2026.
//

#pragma once

#include <optional>

#include "IEditorPanel.h"
#include "../EditorState.h"
#include "Lumiere/Renderer/SceneDesc.h"

namespace sun::ui
{
class SceneHierarchyPanel : public IEditorPanel
{
private:
    struct DelayedDragDrop
    {
        lum::Node3D* source {nullptr};
        lum::Node3D* destination {nullptr};
    };

    // must be replaced by some kind of scene manager in the future. We need access to the currently opened scene
    std::shared_ptr<lum::rdr::SceneDesc> m_scene;
    std::shared_ptr<EditorState> m_state;

    void RenderNode(const lum::Node3D& node, ImGuiTreeNodeFlags flags);
    void RenderLeafNode(const lum::Node3D& node, ImGuiTreeNodeFlags flags);
    std::optional<DelayedDragDrop> HandleDragDrop(const std::unique_ptr<lum::Node3D> &node);
public:
    SceneHierarchyPanel(const std::shared_ptr<lum::rdr::SceneDesc>& scene, const std::shared_ptr<EditorState>& editorState);

    void Render() override;
};
} // sun::ui