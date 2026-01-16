//
// Created by Bellaedris on 13/01/2026.
//

#include "Sunlight.h"

#include "Lumiere/Actor.h"
#include "Lumiere/ResourcesManager.h"

namespace sun
{
    struct CameraData
    {
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };

    void Sunlight::Init()
    {
        m_camera = std::make_unique<lum::Camera>(glm::vec3(0, 0, 0), m_window.AspectRatio(), 70.f, .01f, 1000.f);

        m_cameraData = std::make_unique<lum::gpu::Buffer>(lum::gpu::Buffer::BufferType::Uniform);

        std::vector<lum::gpu::Shader::ShaderSource> shaderSources = {
            {lum::gpu::Shader::Vertex, "shaders/default.vert"},
            {lum::gpu::Shader::Fragment, "shaders/default.frag"}
        };
        lum::ResourcesManager::Instance()->CacheShader(lum::ResourcesManager::PBR_LIT_SHADER_KEY, shaderSources);

        //m_actors.emplace_back("resources/models/Cube/glTF/Cube.gltf");
        m_actors.emplace_back("resources/models/backpack.obj");

        lum::gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
        lum::gpu::GLUtils::SetDepthTesting(true);
    }

    void Sunlight::Render()
    {
        lum::gpu::GLUtils::Clear();

        CameraData cameraData = {m_camera->View(), m_camera->Projection()};

        m_cameraData->Write(sizeof(CameraData), &cameraData, lum::gpu::Buffer::DYNAMIC_DRAW);
        m_cameraData->Bind(0);

        for (const lum::Actor& actor : m_actors)
            actor.Draw();

        ImGui::Begin("tests");
        {
            if (ImGui::Button("Shader Reload"))
                lum::ResourcesManager::Instance()->ShaderHotReload();
        }
        ImGui::End();
    }
} // sun