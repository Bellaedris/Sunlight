//
// Created by Bellaedris on 27/01/2026.
//

#include "RenderSettingsPanel.h"

namespace sun::ui
{
RenderSettingsPanel::RenderSettingsPanel(const std::shared_ptr<lum::rdr::RenderPipeline> &pipeline)
    : m_pipeline(pipeline)
{

}

void RenderSettingsPanel::Render()
{
    ImGui::Begin("Render Settings");
    {
        m_pipeline->RenderUI();
    }
    ImGui::End();
}
} // sun::ui