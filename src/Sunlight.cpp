//
// Created by Bellaedris on 13/01/2026.
//

#include "Sunlight.h"

#include "LumiereConfig.h"
#include "Lumiere/InputManager.h"
#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Renderer/Passes/CompositeNPR.h"
#include "Lumiere/Renderer/Passes/GBuffer.h"
#include "Lumiere/Renderer/Passes/Outline.h"
#include "Lumiere/Renderer/Passes/ShadeNPR.h"
#include "Lumiere/Renderer/Passes/CompositeNPR.h"
#include "Lumiere/Renderer/Passes/ShadePBR.h"
#include "Lumiere/Renderer/Passes/Tonemap.h"
#include "Lumiere/Renderer/Passes/ChromaticAberration.h"
#include "Lumiere/Renderer/Passes/Bloom.h"
#include "Lumiere/Renderer/Passes/CameraSensor.h"
#include "Lumiere/Renderer/Passes/ColorAdjustments.h"
#include "Lumiere/Renderer/Passes/DepthOfField.h"
#include "Lumiere/Renderer/Passes/LensDistortion.h"
#include "Lumiere/Renderer/Passes/Vignette.h"
#include "Lumiere/Utils/MeshLoader.h"

namespace sun
{
Sunlight::Sunlight(int width, int height)
    : App(width, height, 4, 6)
    , m_imguiContext(std::make_unique<ImGuiContext>(m_window))
    , m_rendererManager(std::make_shared<lum::RendererManager>(1, 1, m_internalEvents))
    , m_profilerGPU(std::make_shared<lum::ProfilerGPU>())
    , m_physicsEngine(std::make_unique<lum::PhysicsSystem>())
    , m_scriptEngine(std::make_unique<lum::ScriptEngine>())
    , m_systemProvider(std::make_unique<lum::SystemProvider>(m_physicsEngine.get(), m_scriptEngine.get()))
    , m_scene(std::make_shared<lum::rdr::SceneDesc>(m_systemProvider.get()))
    , m_editor(std::make_unique<Editor>(m_internalEvents, m_scene, m_rendererManager, m_profilerGPU))
{
}

Sunlight::~Sunlight()
{
    // save the render pipeline state upon editor closure
    m_rendererManager->Serialize();
}

void Sunlight::Init()
{
    m_camera = std::make_unique<lum::rdr::Camera>(glm::vec3(0, 0, 0), m_window->AspectRatio(), 70.f, .01f, 100.f);
    m_scene->SetMainCamera(m_camera.get());

    // renderer setup
    lum::rdr::RenderPipeline pbr("pipelinePBR");
    lum::rdr::RenderPipeline npr("pipelineNPR");
    try
    {
        // try to read any existing pipeline
        YAML::Node pipeline = YAML::LoadFile("config/pipelinePBR.yaml");
        pbr.Deserialize(pipeline);
    }
    catch (YAML::BadFile& e)
    {
        // if no pipeline config file is found, create a pipeline from scratch, it will be serialized on editor closure
        pbr.AddPass(std::make_shared<lum::rdr::ShadePBR>(m_window->Width(), m_window->Height()));
        pbr.AddPass(std::make_shared<lum::rdr::CameraSensor>(m_window->Width(), m_window->Height()));
        pbr.AddPass(std::make_shared<lum::rdr::Bloom>(m_window->Width(), m_window->Height()));
        pbr.AddPass(std::make_shared<lum::rdr::DepthOfField>(m_window->Width(), m_window->Height()));
        pbr.AddPass(std::make_shared<lum::rdr::Tonemap>(m_window->Width(), m_window->Height()));
        pbr.AddPass(std::make_shared<lum::rdr::ChromaticAberration>(m_window->Width(), m_window->Height()));
        pbr.AddPass(std::make_shared<lum::rdr::ColorAdjustments>(m_window->Width(), m_window->Height()));
        pbr.AddPass(std::make_shared<lum::rdr::LensDistortion>(m_window->Width(), m_window->Height()));
        pbr.AddPass(std::make_shared<lum::rdr::Vignette>(m_window->Width(), m_window->Height()));
    }

    try
    {
        // try to read any existing pipeline
        YAML::Node pipeline = YAML::LoadFile("config/pipelineNPR.yaml");
        npr.Deserialize(pipeline);
    }
    catch (YAML::BadFile& e)
    {
        // if no pipeline config file is found, create a pipeline from scratch, it will be serialized on editor closure
        npr.AddPass(std::make_shared<lum::rdr::Outline>(m_window->Width(), m_window->Height()));
        npr.AddPass(std::make_shared<lum::rdr::ShadeNPR>(m_window->Width(), m_window->Height()));
        npr.AddPass(std::make_shared<lum::rdr::CompositeNPR>(m_window->Width(), m_window->Height()));
    }

    m_rendererManager->AddPipeline(pbr);
    m_rendererManager->AddPipeline(npr);
    m_rendererManager->SetActivePipeline(0);

    lum::gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
    lum::gpu::GLUtils::SetDepthTesting(true);

    m_editor->SetViewportCamera(m_camera.get());
}

void Sunlight::Render()
{
    lum::gpu::GLUtils::Clear();
    m_imguiContext->BeginFrame();

    lum::rdr::FrameData frame
    {
        .scene = m_scene,
        .frameIndex = m_frameIndex,
        .profilerGPU = m_profilerGPU
    };

    if (lum::InputManager::IsMouseButtonPressed(lum::MouseButton::lRightClick))
    {
        m_profilerGPU->Reset();
        m_rendererManager->SetActivePipeline((renderer + 1) % 2);
        renderer += 1;
    }

    m_scriptEngine->Update(m_deltaTime);
    m_scene->RootNode()->Update(m_deltaTime);
    m_profilerGPU->BeginFrame();
    m_rendererManager->Render(frame);
    m_profilerGPU->EndFrame();

    m_editor->Render();

    m_imguiContext->EndFrame();
}

void Sunlight::RenderUI()
{

}
} // sun