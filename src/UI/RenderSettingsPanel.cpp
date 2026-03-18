//
// Created by Bellaedris on 27/01/2026.
//

#include "RenderSettingsPanel.h"

#include "Lumiere/ResourcesManager.h"

namespace sun::ui
{
RenderSettingsPanel::RenderSettingsPanel(const std::shared_ptr<lum::RendererManager> &renderer)
    : m_pipeline(renderer)
{

}

void RenderSettingsPanel::Render()
{
    ImGui::Begin("Render Settings");
    {
        m_pipeline->RenderUI();
        if (ImGui::Button("Shader Reload"))
            lum::ResourcesManager::Instance()->ShaderHotReload();
    }
    ImGui::End();
}
} // sun::ui