//
// Created by Bellaedris on 26/01/2026.
//

#pragma once
#include <memory>
#include <vector>

#include "EditorState.h"
#include "Lumiere/Events/EventHandler.h"
#include "Lumiere/Renderer/RenderPipeline.h"
#include "Lumiere/Renderer/SceneDesc.h"
#include "UI/IEditorPanel.h"

namespace sun
{
class Editor
{
private:
    std::vector<std::unique_ptr<ui::IEditorPanel>> m_panels;
    std::shared_ptr<lum::evt::EventHandler> m_events;
    std::shared_ptr<EditorState> m_state;
    std::shared_ptr<lum::rdr::SceneDesc> m_scene;
    std::shared_ptr<lum::rdr::RenderPipeline> m_pipeline;
public:
    Editor(const std::shared_ptr<lum::evt::EventHandler>& events, const std::shared_ptr<lum::rdr::SceneDesc>& scene, const std::shared_ptr<lum::rdr::RenderPipeline>& pipeline);
    void Render();
    void OnEvent(const std::shared_ptr<lum::evt::IEvent>& event);
};
} // sun