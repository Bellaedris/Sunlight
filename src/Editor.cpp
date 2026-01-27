//
// Created by Bellaedris on 26/01/2026.
//

#include "Editor.h"

#include <iostream>

#include "imgui/imgui.h"
#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Events/RenderEvents.h"
#include "UI/ViewportPanel.h"

namespace sun
{
Editor::Editor(const std::shared_ptr<lum::evt::EventHandler>& events, const std::shared_ptr<lum::rdr::SceneDesc>& scene)
    : m_events(events)
    , m_scene(scene)
    , m_state(std::make_shared<EditorState>())
{
    m_events->Subscribe(lum::evt::EventType::FrameRendered, std::bind(&Editor::OnEvent, this, std::placeholders::_1));

    m_panels.emplace_back(std::make_unique<ui::ViewportPanel>(m_state));
}

void Editor::Render()
{
    ImGui::DockSpaceOverViewport();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    for (const auto& panel : m_panels)
        panel->Render();
    ImGui::PopStyleVar(1);

    ImGui::Begin("tests");
    {
        if (ImGui::Button("Shader Reload"))
            lum::ResourcesManager::Instance()->ShaderHotReload();
    }
    ImGui::End();

    // debug-only view
    /*ImGui::SetNextWindowPos({0, 0}, 0, {0, 0});
    ImGui::Begin(
            "Infos",
            nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize
    );
    {
        ImGui::Text("Lumiere version %d.%d", LUMIERE_VERSION_MAJOR, LUMIERE_VERSION_MINOR);
        ImGui::Text("OpenGL%s", api);
        ImGui::Text("%s %s", vendor, gpu);

        float average = 0.0f;
        for (float n: framerate)
            average += n;
        average /= (float) IM_ARRAYSIZE(framerate);
        ImGui::Text("Average %02fFPS", average);
        uint64_t elapsedGPU = m_gpuDeltaTime.Elapsed();
        // ugly way to convert to a readable format but I'm kinda lazy and don't want to fight both
        // std::chrono types and c strings
        float cpuSeconds = m_deltaTime * 1000.f;
        int milli = (int) (elapsedGPU / 1000000);
        int micro = (int) ((elapsedGPU / 1000) % 1000);
        ImGui::Text(
                "cpu %03fms\ngpu %02dms % 03dus",
                cpuSeconds,
                milli, micro
        );
        ImGui::End();
        }*/

    if (m_state->temp.shouldNotifyViewportChange)
    {
        m_events->Emit(std::make_shared<lum::evt::FramebufferResizedEvent>(m_state->persistent.viewportSize));
        m_scene->Camera()->SetAspect(static_cast<float>(m_state->persistent.viewportSize.x) / static_cast<float>(m_state->persistent.viewportSize.y));
        m_state->temp.shouldNotifyViewportChange = false;
    }
}

void Editor::OnEvent(const std::shared_ptr<lum::evt::IEvent> &event)
{
    for (const auto& panel : m_panels)
        panel->OnEvent(event);
}
} // sun
