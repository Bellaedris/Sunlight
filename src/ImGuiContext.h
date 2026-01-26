//
// Created by Bellaedris on 26/01/2026.
//

#pragma once

#include <memory>
#include <Lumiere/Window.h>

namespace sun
{
class ImGuiContext
{
private:
    std::shared_ptr<lum::Window> m_window;
public:
    explicit ImGuiContext(const std::shared_ptr<lum::Window>& window);
    ~ImGuiContext();

    void BeginFrame();
    void EndFrame();
};
} // sun