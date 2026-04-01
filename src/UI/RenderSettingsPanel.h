//
// Created by Bellaedris on 27/01/2026.
//

#pragma once
#include "IEditorPanel.h"
#include "Lumiere/RendererManager.h"
#include "../EditorState.h"

namespace sun::ui
{
class RenderSettingsPanel : public IEditorPanel
{
private:
    std::shared_ptr<EditorState> m_state;
public:
    RenderSettingsPanel(const std::shared_ptr<EditorState>& state);
    void Render() override;
};
} // sun::ui