//
// Created by Bellaedris on 26/01/2026.
//

#pragma once
#include "IEditorPanel.h"
#include "../EditorState.h"
#include "imgui/imgui.h"
#include "Lumiere/GPU/Texture.h"

namespace sun::ui
{
class ViewportPanel : public IEditorPanel
{
private:
    std::shared_ptr<lum::gpu::Texture> m_lastRenderedFrame;
    ImVec2 m_lastViewportSize;

    std::shared_ptr<EditorState> m_state;

    bool m_isResizing {false};
public:
    ViewportPanel(const std::shared_ptr<EditorState>& editorState);
    void Render() override;
    void OnEvent(const std::shared_ptr<lum::evt::IEvent>& e) override;
};
} // sun::ui