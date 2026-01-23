//
// Created by Bellaedris on 13/01/2026.
//

#include "Sunlight.h"

#include "Lumiere/Actor.h"
#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Renderer/Passes/GBuffer.h"
#include "Lumiere/Renderer/Passes/Outline.h"
#include "Lumiere/Renderer/Passes/ShadeNPR.h"
#include "Lumiere/Utils/MeshLoader.h"

namespace sun
{
    void Sunlight::Init()
    {
        m_camera = std::make_unique<lum::rdr::Camera>(glm::vec3(0, 0, 0), m_window.AspectRatio(), 70.f, .01f, 1000.f);
        m_scene.SetMainCamera(m_camera.get());

        m_scene.AddMesh("resources/models/DamagedHelmet/glTF/DamagedHelmet.gltf");
        //m_actors.emplace_back("resources/models/backpack.obj");

        m_scene.Lights()->AddDirLight({-5, -1, -2}, 1.f, {1, 1, 1});

        // renderer setup
        m_renderer.AddPass(new lum::rdr::GBuffer(m_window.Width(), m_window.Height()));
        m_renderer.AddPass(new lum::rdr::Outline(m_window.Width(), m_window.Height()));
        m_renderer.AddPass(new lum::rdr::ShadeNPR());

        lum::gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
        lum::gpu::GLUtils::SetDepthTesting(true);
    }

    void Sunlight::Render()
    {
        lum::gpu::GLUtils::Clear();

        m_renderer.Render(m_scene);

        ImGui::Begin("tests");
        {
            if (ImGui::Button("Shader Reload"))
                lum::ResourcesManager::Instance()->ShaderHotReload();
        }
        ImGui::End();
    }
} // sun