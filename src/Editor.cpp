//
// Created by Bellaedris on 26/01/2026.
//

#include "Editor.h"

#include <iostream>

#include "imgui/imgui.h"
#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Events/RenderEvents.h"
#include "UI/InspectorPanel.h"
#include "UI/ProfilerPanel.h"
#include "UI/RenderSettingsPanel.h"
#include "UI/SceneHierarchyPanel.h"
#include "UI/ViewportPanel.h"

namespace sun
{
Editor::Editor
(
    const std::shared_ptr<lum::evt::EventHandler> &  events,
    const std::shared_ptr<lum::rdr::SceneDesc> &     scene,
    const std::shared_ptr<lum::RendererManager> &renderer,
    const std::shared_ptr<lum::ProfilerGPU>& profiler
)
    : m_events(events)
    , m_state(std::make_shared<EditorState>())
    , m_scene(scene)
    , m_pipeline(renderer)
{
    LUM_SUB_TO_EVENT(m_events, lum::evt::EventType::FrameRendered, Editor::OnEvent);
    LUM_SUB_TO_EVENT(m_events, lum::evt::WindowResized, Editor::OnEvent);

    m_panels.emplace_back(std::make_unique<ui::ViewportPanel>(m_state));
    m_panels.emplace_back(std::make_unique<ui::RenderSettingsPanel>(m_pipeline));
    m_panels.emplace_back(std::make_unique<ui::ProfilerPanel>(profiler));
    m_panels.emplace_back(std::make_unique<ui::SceneHierarchyPanel>(m_scene, m_state));
    m_panels.emplace_back(std::make_unique<ui::InspectorPanel>(m_state));

    m_state->Deserialize();
    if (m_state->persistent.activeScenePath.empty() == false)
        m_scene->Deserialize(m_state->persistent.activeScenePath);
}

Editor::~Editor()
{
    m_scene->Serialize();
    m_state->Serialize();
}

void Editor::Render()
{
    ImGui::DockSpaceOverViewport();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 3.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

    for (const auto& panel : m_panels)
        panel->Render();
    ImGui::PopStyleVar(3);

    ImGui::Begin("tests");
    {
        if (ImGui::Button("Shader Reload"))
            lum::ResourcesManager::Instance()->ShaderHotReload();
    }
    ImGui::End();

    if (m_state->temp.shouldNotifyViewportChange)
    {
        m_events->Emit(std::make_shared<lum::evt::FramebufferResizedEvent>(m_state->temp.viewportSize));
        m_scene->Camera()->SetAspect(static_cast<float>(m_state->temp.viewportSize.x) / static_cast<float>(m_state->temp.viewportSize.y));
        m_state->temp.shouldNotifyViewportChange = false;
    }
}

void Editor::OnEvent(const std::shared_ptr<lum::evt::IEvent> &event)
{
    for (const auto& panel : m_panels)
        panel->OnEvent(event);
}

void Editor::SetViewportCamera(lum::rdr::Camera *camera)
{
    m_state->temp.viewportCamera = camera;
}
} // sun
