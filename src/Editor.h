//
// Created by Bellaedris on 26/01/2026.
//

#pragma once
#include <memory>
#include <vector>

#include "EditorState.h"
#include "Lumiere/RendererManager.h"
#include "Lumiere/Systems/CameraSystem.h"
#include "Lumiere/Events/EventHandler.h"
#include "Lumiere/Renderer/RenderPipeline.h"
#include "Lumiere/Renderer/SceneDesc.h"
#include "UI/IEditorPanel.h"

namespace sun
{
class Editor
{
private:
    std::unique_ptr<lum::rdr::Camera> m_editorCamera {nullptr};

    std::vector<std::unique_ptr<ui::IEditorPanel>> m_panels;
    std::shared_ptr<lum::evt::EventHandler> m_events;
    std::shared_ptr<EditorState> m_state;
    std::shared_ptr<lum::rdr::SceneDesc> m_scene;
    lum::RendererManager* m_pipeline;
    lum::CameraSystem* m_cameraSystem;
public:
    Editor(const std::shared_ptr<lum::evt::EventHandler>& events, const std::shared_ptr<lum::rdr::SceneDesc>& scene, lum::SystemProvider* systems, const std::shared_ptr<lum::ProfilerGPU>& profiler);
    ~Editor();

    void Render(float dt);
    void OnEvent(const std::shared_ptr<lum::evt::IEvent>& event);

    void SetViewportCamera(lum::rdr::Camera* camera);
};
} // sun