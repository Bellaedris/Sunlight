//
// Created by Bellaedris on 26/01/2026.
//

#pragma once
#include "IEditorPanel.h"
#include "../EditorState.h"
#include "imgui/imgui.h"
#include "Lumiere/GPU/Texture.h"
#include "Lumiere/Renderer/SceneDesc.h"

namespace sun::ui
{
class ViewportPanel : public IEditorPanel
{
private:
    std::shared_ptr<lum::gpu::Texture> m_lastRenderedFrame;
    ImVec2 m_lastViewportSize;

    std::shared_ptr<EditorState> m_state;
    std::shared_ptr<lum::rdr::SceneDesc> m_scene;

    bool m_windowResized {false};
    bool m_isResizing    {false};
    bool m_initialized   {false};

    void RegisterGuizmos();
public:
    ViewportPanel(const std::shared_ptr<EditorState>& editorState, const std::shared_ptr<lum::rdr::SceneDesc>& scene);
    void Render() override;
    void OnEvent(const std::shared_ptr<lum::evt::IEvent>& e) override;
};
} // sun::ui