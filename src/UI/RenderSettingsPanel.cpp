//
// Created by Bellaedris on 27/01/2026.
//

#include "RenderSettingsPanel.h"

#include "Lumiere/ResourcesManager.h"

namespace sun::ui
{
RenderSettingsPanel::RenderSettingsPanel(const std::shared_ptr<EditorState>& state)
    : m_state(state)
{

}

void RenderSettingsPanel::Render()
{
    ImGui::Begin("Render Settings");
    {
        m_state->temp.systems->m_renderer->RenderUI();
        if (ImGui::Button("Shader Reload"))
            lum::ResourcesManager::Instance()->ShaderHotReload();
    }
    ImGui::End();
}
} // sun::ui