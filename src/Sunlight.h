//
// Created by Bellaedris on 13/01/2026.
//

#pragma once

#include <Lumiere/App.h>

#include "Lumiere/Renderer/RenderPipeline.h"
#include "Lumiere/Renderer/SceneDesc.h"

namespace sun
{
    class Sunlight : public lum::App
    {
    public:
        Sunlight(int width, int height) : App(width, height, 4, 6) {};

    private:
        #pragma region Members
        lum::rdr::SceneDesc m_scene;
        lum::rdr::RenderPipeline m_renderer;
        #pragma endregion Members

        void Init() override;

        void Render() override;
    };
} // sun