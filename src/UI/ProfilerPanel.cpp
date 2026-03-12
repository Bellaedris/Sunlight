//
// Created by Bellaedris on 15/02/2026.
//

#include "ProfilerPanel.h"

#include <numeric>

namespace sun::ui
{
ProfilerPanel::ProfilerPanel(std::shared_ptr<lum::ProfilerGPU> profiler)
    : m_profiler(profiler)
{
    for (int i = 0; i < lum::ProfilerGPU::MAX_FRAMES_STORAGE; i++)
    {
        m_labelsStr[i] = std::to_string(i);
        m_labels[i] = m_labelsStr[i].c_str();
    }
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

        // prepare data for plotting and displaying
        std::vector<const char*> ilabels(markerCount);
        double positions[lum::ProfilerGPU::MAX_FRAMES_STORAGE];

        std::vector<float> datas(lum::ProfilerGPU::MAX_FRAMES_STORAGE * markerCount);

        size_t frameCount = frameStats.size();
        std::vector<float> means(markerCount, .0f);
        for (int i = 0; i < frameCount; i++)
        {
            positions[i] = static_cast<double>(i);

            for (int j = 0; j < frameStats[i].elapsedTimes.size(); j++)
            {
                if (i == 0)
                    ilabels[j] = frameStats[0].scopeHandles[j].c_str();
                uint64_t passTime = frameStats[i].elapsedTimes[j];
                //convert to milliseconds
                float timeMs = static_cast<float>(passTime) / 1000000.f;
                datas[j * frameCount + i] = timeMs;
                means[j] += timeMs;
            }
        }

        std::transform(means.begin(), means.end(), means.begin(), [&](float val){ return val / lum::ProfilerGPU::MAX_FRAMES_STORAGE; });
        float meanFrameTotal = std::accumulate(means.begin(), means.end(), .0f);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull;

        ImGui::PushID("Means");
        bool opened = ImGui::TreeNodeEx("", flags);
        ImGui::SameLine();
        ImGui::Text("Frame - %lf ms", meanFrameTotal);
        if (opened)
        {
            for (int i = 0; i < markerCount; i++)
            {
                ImGui::Text("%s - %lf ms", ilabels[i], means[i]);
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
        if (ImPlot::BeginPlot("Frame profile"))
        {
            ImPlot::SetupLegend(ImPlotLocation_East, ImPlotLegendFlags_Outside);
            ImPlot::SetupAxes("Frames", "Execution times (ms)",ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisTicks(ImAxis_X1, positions, static_cast<int>(frameCount), m_labels);
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