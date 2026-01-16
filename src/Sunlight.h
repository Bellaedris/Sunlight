//
// Created by Bellaedris on 13/01/2026.
//

#pragma once

#include <Lumiere/App.h>
#include "Lumiere/Actor.h"

namespace sun
{
    class Sunlight : public lum::App
    {
    public:
        Sunlight(int width, int height) : App(width, height, 4, 6) {};

    private:
        #pragma region Members
        std::vector<lum::Actor> m_actors;
        std::unique_ptr<lum::gpu::Buffer> m_cameraData;
        #pragma endregion Members

        void Init() override;

        void Render() override;
    };
} // sun