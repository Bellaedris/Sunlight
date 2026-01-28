//
// Created by Bellaedris on 27/01/2026.
//

#pragma once
#include "IEditorPanel.h"
#include "Lumiere/Renderer/RenderPipeline.h"

namespace sun::ui
{
class RenderSettingsPanel : public IEditorPanel
{
private:
    std::shared_ptr<lum::rdr::RenderPipeline> m_pipeline;
public:
    RenderSettingsPanel(const std::shared_ptr<lum::rdr::RenderPipeline>& pipeline);
    void Render() override;
};
} // sun::ui