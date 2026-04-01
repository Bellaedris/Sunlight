//
// Created by Bellaedris on 13/01/2026.
//

#pragma once

#include <Lumiere/App.h>

#include "Editor.h"
#include "ImGuiContext.h"
#include "Lumiere/Systems/PhysicsSystem.h"
#include "Lumiere/Systems/ScriptEngine.h"
#include "Lumiere/RendererManager.h"
#include "Lumiere/Events/EventHandler.h"
#include "Lumiere/Renderer/RenderPipeline.h"
#include "Lumiere/Renderer/SceneDesc.h"
#include "Lumiere/Systems/CameraSystem.h"

namespace sun
{
    class Sunlight : public lum::App
    {
    public:
        Sunlight(int width, int height);
        ~Sunlight();

    private:
        #pragma region Members
        std::unique_ptr<ImGuiContext> m_imguiContext;

        std::shared_ptr<lum::RendererManager> m_rendererManager;
        std::shared_ptr<lum::ProfilerGPU> m_profilerGPU;
        // always create systems with ScriptEngine last
        std::unique_ptr<lum::PhysicsSystem> m_physicsEngine;
        std::unique_ptr<lum::CameraSystem> m_cameraSystem;
        std::unique_ptr<lum::ScriptEngine> m_scriptEngine;
        std::unique_ptr<lum::SystemProvider> m_systemProvider;
        std::shared_ptr<lum::rdr::SceneDesc> m_scene;
        std::unique_ptr<Editor> m_editor;

        int renderer {0};
        #pragma endregion Members

        void Init() override;

        void Render() override;

        void RenderUI() override;
    };
} // sun