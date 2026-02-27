//
// Created by Bellaedris on 13/01/2026.
//

#pragma once

#include <Lumiere/App.h>

#include "Editor.h"
#include "ImGuiContext.h"
#include "Lumiere/RendererManager.h"
#include "Lumiere/Events/EventHandler.h"
#include "Lumiere/Renderer/RenderPipeline.h"
#include "Lumiere/Renderer/SceneDesc.h"

namespace sun
{
    class Sunlight : public lum::App
    {
    public:
        Sunlight(int width, int height);
        ~Sunlight();

    private:
        #pragma region Members
        std::shared_ptr<lum::evt::EventHandler> m_internalEvents;
        std::unique_ptr<ImGuiContext> m_imguiContext;
        std::shared_ptr<lum::rdr::SceneDesc> m_scene;

        std::shared_ptr<lum::RendererManager> m_rendererManager;
        std::shared_ptr<lum::rdr::RenderPipeline> m_activeRenderer {nullptr};
        std::shared_ptr<lum::rdr::RenderPipeline> m_rendererNPR;
        std::shared_ptr<lum::rdr::RenderPipeline> m_renderer;
        std::shared_ptr<lum::ProfilerGPU> m_profilerGPU;
        std::unique_ptr<Editor> m_editor;

        int renderer {0};
        #pragma endregion Members

        void Init() override;

        void Render() override;

        void RenderUI() override;
    };
} // sun