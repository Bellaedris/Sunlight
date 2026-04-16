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
    lum::SystemProvider* systems,
    const std::shared_ptr<lum::ProfilerGPU>& profiler
)
    : m_events(events)
    , m_state(std::make_shared<EditorState>())
    , m_scene(scene)
{
    LUM_SUB_TO_EVENT(m_events, lum::evt::EventType::FrameRendered, Editor::OnEvent);
    LUM_SUB_TO_EVENT(m_events, lum::evt::WindowResized, Editor::OnEvent);

    // we can use any aspect ratio here, the camera will be resized on initial viewport creation
    m_editorCamera = std::make_unique<lum::rdr::Camera>(glm::vec3(0, 0, 0), 16.f / 9.f, 70.f, .01f, 100.f);
    m_editorCamera->SetSensitivity(1.f);
    m_scene->SetMainCamera(m_editorCamera.get());

    m_state->temp.systems = systems;
    m_state->temp.viewportCamera = m_editorCamera.get();
    m_state->temp.systems->m_camera->SetEditorCamera(m_editorCamera.get());
    m_state->Deserialize();
    if (m_state->persistent.activeScenePath.empty() == false)
        m_scene->Deserialize(m_state->persistent.activeScenePath);

    m_panels.emplace_back(std::make_unique<ui::ViewportPanel>(m_state, m_scene));
    m_panels.emplace_back(std::make_unique<ui::RenderSettingsPanel>(m_state));
    m_panels.emplace_back(std::make_unique<ui::ProfilerPanel>(profiler));
    m_panels.emplace_back(std::make_unique<ui::SceneHierarchyPanel>(m_scene, m_state));
    m_panels.emplace_back(std::make_unique<ui::InspectorPanel>(m_state));
}

Editor::~Editor()
{
    m_scene->Serialize();
    m_state->Serialize();
}

void Editor::Render(float dt)
{
    m_state->temp.deltaTime = dt;
    ImGui::DockSpaceOverViewport();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 3.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

    for (const auto& panel : m_panels)
        panel->Render();
    ImGui::PopStyleVar(3);

    if (m_state->temp.shouldNotifyViewportChange)
    {
        m_events->Emit(std::make_shared<lum::evt::FramebufferResizedEvent>(m_state->temp.viewportSize));
        m_editorCamera->SetAspect(static_cast<float>(m_state->temp.viewportSize.x) / static_cast<float>(m_state->temp.viewportSize.y));
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
