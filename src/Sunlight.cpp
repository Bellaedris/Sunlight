//
// Created by Bellaedris on 13/01/2026.
//

#include "Sunlight.h"

#include "LumiereConfig.h"
#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Renderer/Passes/GBuffer.h"
#include "Lumiere/Renderer/Passes/Outline.h"
#include "Lumiere/Renderer/Passes/ShadeNPR.h"
#include "Lumiere/Utils/MeshLoader.h"

namespace sun
{
Sunlight::Sunlight(int width, int height)
    : App(width, height, 4, 6)
    , m_internalEvents(std::make_shared<lum::evt::EventHandler>())
    , m_imguiContext(std::make_unique<ImGuiContext>(m_window))
    , m_scene(std::make_shared<lum::rdr::SceneDesc>())
    , m_renderer(std::make_unique<lum::rdr::RenderPipeline>(m_internalEvents))
    , m_editor(std::make_unique<Editor>(m_internalEvents, m_scene))
{
}

void Sunlight::Init()
{
    m_camera = std::make_unique<lum::rdr::Camera>(glm::vec3(0, 0, 0), m_window->AspectRatio(), 70.f, .01f, 1000.f);
    m_scene->SetMainCamera(m_camera.get());

    m_scene->AddMesh("resources/models/DamagedHelmet/glTF/DamagedHelmet.gltf");
    //m_actors.emplace_back("resources/models/backpack.obj");

    m_scene->Lights()->AddDirLight({-5, -1, -2}, 1.f, {1, 1, 1});

    // renderer setup
    m_renderer->AddPass(new lum::rdr::GBuffer(m_window->Width(), m_window->Height()));
    m_renderer->AddPass(new lum::rdr::Outline(m_window->Width(), m_window->Height()));
    m_renderer->AddPass(new lum::rdr::ShadeNPR(m_window->Width(), m_window->Height()));

    lum::gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
    lum::gpu::GLUtils::SetDepthTesting(true);
}

void Sunlight::Render()
{
    lum::gpu::GLUtils::Clear();

    m_renderer->Render(*m_scene);
}

void Sunlight::RenderUI()
{
    m_imguiContext->BeginFrame();
    m_editor->Render();
    m_imguiContext->EndFrame();
}
} // sun