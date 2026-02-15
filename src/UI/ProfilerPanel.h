//
// Created by Bellaedris on 15/02/2026.
//

#pragma once
#include "IEditorPanel.h"
#include "Lumiere/ProfilerGPU.h"

namespace sun::ui
{
class ProfilerPanel : public IEditorPanel
{
private:
    std::shared_ptr<lum::ProfilerGPU> m_profiler;
public:
    ProfilerPanel(std::shared_ptr<lum::ProfilerGPU> profiler);
    void Render() override;
};
} // sun::ui