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
    , m_internalEvents(std::make_shared<lum::evt::EventHandler>())
    , m_imguiContext(std::make_unique<ImGuiContext>(m_window))
    , m_scene(std::make_shared<lum::rdr::SceneDesc>())
    , m_rendererManager(std::make_shared<lum::RendererManager>(1, 1, m_internalEvents))
    , m_renderer(std::make_shared<lum::rdr::RenderPipeline>("pipelinePBR"))
    , m_rendererNPR(std::make_shared<lum::rdr::RenderPipeline>("pipelineNPR"))
    , m_profilerGPU(std::make_shared<lum::ProfilerGPU>())
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

    m_scene->AddMesh("resources/models/Sponza/glTF/Sponza.gltf");
    //m_scene->AddMesh("resources/models/bistrogltf/bistro.gltf");
    //m_actors.emplace_back("resources/models/backpack.obj");

    m_scene->Lights()->AddDirLight({0, -1, -.5}, 3.f, {1, 1, 1});

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

    m_activeRenderer = m_renderer;

    lum::gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
    lum::gpu::GLUtils::SetDepthTesting(true);
}

void Sunlight::Render()
{
    lum::gpu::GLUtils::Clear();

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

    // if (lum::InputManager::IsMouseButtonReleased(lum::MouseButton::lRightClick))
    // {
    //     m_profilerGPU->Reset();
    //     m_rendererManager->SetActivePipeline(0);
    // }
    m_scene->RootNode()->Update();
    m_profilerGPU->BeginFrame();
    m_rendererManager->Render(frame);
    m_profilerGPU->EndFrame();
}

void Sunlight::RenderUI()
{
    m_imguiContext->BeginFrame();
    // ImGui::BeginMainMenuBar();
    // if (ImGui::BeginMenu("Stats"))
    // {
    //     // debug-only view
    //     ImGui::SetNextWindowPos({0, 0}, 0, {0, 0});
    //     ImGui::Begin(
    //             "Infos",
    //             nullptr,
    //             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking |
    //             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize
    //     );
    //     {
    //         ImGui::Text("Lumiere version %d.%d", LUMIERE_VERSION_MAJOR, LUMIERE_VERSION_MINOR);
    //         ImGui::Text("OpenGL%s", api);
    //         ImGui::Text("%s %s", vendor, gpu);
    //
    //         float average = 0.0f;
    //         for (float n: framerate)
    //             average += n;
    //         average /= (float) IM_ARRAYSIZE(framerate);
    //         ImGui::Text("Average %02fFPS", average);
    //         uint64_t elapsedGPU = m_gpuDeltaTime.Elapsed();
    //         // ugly way to convert to a readable format but I'm kinda lazy and don't want to fight both
    //         // std::chrono types and c strings
    //         float cpuSeconds = m_deltaTime * 1000.f;
    //         int milli = (int) (elapsedGPU / 1000000);
    //         int micro = (int) ((elapsedGPU / 1000) % 1000);
    //         ImGui::Text(
    //                 "cpu %03fms\ngpu %02dms % 03dus",
    //                 cpuSeconds,
    //                 milli, micro
    //         );
    //         ImGui::End();
    //     }
    //     ImGui::EndMenu();
    // }
    // ImGui::EndMainMenuBar();
    m_editor->Render();
    m_imguiContext->EndFrame();
}
} // sun