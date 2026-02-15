//
// Created by Bellaedris on 15/02/2026.
//

#include "ProfilerPanel.h"

namespace sun::ui
{
ProfilerPanel::ProfilerPanel(std::shared_ptr<lum::ProfilerGPU> profiler)
    : m_profiler(profiler)
{

}

void ProfilerPanel::Render()
{
    ImGui::Begin("Profiler");
    {
        const std::vector<lum::ProfilerGPU::FrameStats> frameStats = m_profiler->Data();
        size_t markerCount = frameStats[0].elapsedTimes.size();
        if (markerCount == 0)
        {
            ImGui::End();
            return;
        }

        // prepare data for plotting
        std::vector<const char*> ilabels(markerCount);
        const char* glabels[lum::ProfilerGPU::MAX_FRAMES_STORAGE];
        double positions[lum::ProfilerGPU::MAX_FRAMES_STORAGE];

        std::vector<float> datas(lum::ProfilerGPU::MAX_FRAMES_STORAGE * markerCount);

        size_t frameCount = frameStats.size() - 1;
        for (int i = 0; i < frameCount; i++)
        {
            glabels[i] = std::to_string(i).c_str();
            positions[i] = static_cast<double>(i);

            for (int j = 0; j < frameStats[i].elapsedTimes.size(); j++)
            {
                if (i == 0)
                    ilabels[j] = frameStats[0].scopeHandles[j].c_str();
                uint64_t passTime = frameStats[i].elapsedTimes[j];
                //convert to milliseconds
                datas[j * frameCount + i] = static_cast<float>(passTime) / 1000000.f;
            }
        }

        if (ImPlot::BeginPlot("Frame profile"))
        {
            ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
            ImPlot::SetupAxes("Frames", "Execution times (ms)",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisTicks(ImAxis_X1, positions, static_cast<int>(frameCount), glabels);
            ImPlot::PlotBarGroups
            (
                ilabels.data(),
                datas.data(),
                markerCount,
                lum::ProfilerGPU::MAX_FRAMES_STORAGE
            );
            ImPlot::EndPlot();
        }
    }
    ImGui::End();
}
} // sun::ui