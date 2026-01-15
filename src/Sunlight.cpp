//
// Created by Bellaedris on 13/01/2026.
//

#include "Sunlight.h"

#include "Lumiere/Actor.h"

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

        m_mainShader = std::make_unique<lum::gpu::Shader>();
        m_mainShader->AddShaderFromFile(lum::gpu::Shader::Fragment, "shaders/default.frag");
        m_mainShader->AddShaderFromFile(lum::gpu::Shader::Vertex, "shaders/default.vert");
        m_mainShader->Create();

        m_actors.emplace_back("resources/models/Cube/glTF/Cube.gltf");

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
    }
} // sun