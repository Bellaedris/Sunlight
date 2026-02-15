//
// Created by Bellaedris on 26/01/2026.
//

#include "Editor.h"

#include <iostream>

#include "imgui/imgui.h"
#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Events/RenderEvents.h"
#include "UI/ProfilerPanel.h"
#include "UI/RenderSettingsPanel.h"
#include "UI/ViewportPanel.h"

namespace sun
{
Editor::Editor
(
    const std::shared_ptr<lum::evt::EventHandler> &  events,
    const std::shared_ptr<lum::rdr::SceneDesc> &     scene,
    const std::shared_ptr<lum::rdr::RenderPipeline> &pipeline,
    const std::shared_ptr<lum::ProfilerGPU>& profiler
)
    : m_events(events)
    , m_state(std::make_shared<EditorState>())
    , m_scene(scene)
    , m_pipeline(pipeline)
{
    LUM_SUB_TO_EVENT(m_events, lum::evt::EventType::FrameRendered, Editor::OnEvent);

    m_panels.emplace_back(std::make_unique<ui::ViewportPanel>(m_state));
    m_panels.emplace_back(std::make_unique<ui::RenderSettingsPanel>(m_pipeline));
    m_panels.emplace_back(std::make_unique<ui::ProfilerPanel>(profiler));
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
