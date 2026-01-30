//
// Created by Bellaedris on 26/01/2026.
//

#pragma once

#include <Lumiere/Events/IEvent.h>
#include <memory>
#include <imgui/imgui.h>

namespace sun::ui
{
class IEditorPanel
{
public:
    virtual ~IEditorPanel() = default;

    virtual void Render() = 0;
    virtual void OnEvent(const std::shared_ptr<lum::evt::IEvent>& e) {};
};
} // lum::ui