//
// Created by belle on 24/02/2026.
//

#pragma once
#include "IEditorPanel.h"
#include "../EditorState.h"

namespace sun::ui
{
class InspectorPanel : public IEditorPanel
{
private:
    std::shared_ptr<EditorState> m_state;
    int m_selectedComponentType {0};
    lum::Node3D* m_selected;

    constexpr static std::array<const char*, 1> COMPONENT_TYPES  = {"Mesh Renderer"};
public:
    InspectorPanel(const std::shared_ptr<EditorState>& state);

    void Render() override;
};
} // sun::ui