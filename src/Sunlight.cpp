//
// Created by Bellaedris on 13/01/2026.
//

#include "Sunlight.h"

namespace sun
{
    void Sunlight::Init()
    {
        m_camera = std::make_unique<lum::Camera>(glm::vec3(0, 0, 0), m_window.AspectRatio(), 70.f, .01f, 1000.f);

        lum::gpu::GLUtils::ClearColor({.2f, .2f, .2f, 1.f});
        lum::gpu::GLUtils::SetDepthTesting(true);
    }

    void Sunlight::Render()
    {
        lum::gpu::GLUtils::Clear();
    }
} // sun