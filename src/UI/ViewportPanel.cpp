//
// Created by Bellaedris on 26/01/2026.
//

#include "ViewportPanel.h"

#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Events/RenderEvents.h"
#include "Lumiere/Renderer/RenderPipeline.h"
#include "Lumiere/Renderer/Passes/ShadeNPR.h"

namespace sun::ui {
ViewportPanel::ViewportPanel(const std::shared_ptr<EditorState>& editorState)
    : m_state(editorState)
{

}

void ViewportPanel::Render()
{
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        ImVec2 availableSize = ImGui::GetContentRegionAvail();

        if (m_lastViewportSize.x != availableSize.x || m_lastViewportSize.y != availableSize.y)
        {
            // initial resizing: the 1st render call has lastViewportSize == 0, but the available size seems to ignore the dockspace...
            // we have to wait the second frame to resize, which is the first non-zero lastViewportSize.
            if (m_lastViewportSize.x != 0 && m_initialized == false)
            {
                m_state->temp.shouldNotifyViewportChange = true;
                m_state->persistent.viewportSize = {availableSize.x, availableSize.y};
                m_isResizing = false;
                m_initialized = true;
            }
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                m_isResizing = true;
            }
        }

        if (m_isResizing == true && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            m_state->temp.shouldNotifyViewportChange = true;
            m_state->persistent.viewportSize = {availableSize.x, availableSize.y};
            m_isResizing = false;
        }
        m_lastViewportSize = availableSize;
        glm::ivec2 size = m_lastRenderedFrame->Size();
        // we change the UVs because of OpenGL's flipped Y axis
        ImGui::Image(m_lastRenderedFrame->Handle(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}

void ViewportPanel::OnEvent(const std::shared_ptr<lum::evt::IEvent> &e)
{
    if (e->Type() == lum::evt::FrameRendered)
    {
        auto event = dynamic_pointer_cast<lum::evt::FrameRenderedEvent>(e);
        m_lastRenderedFrame = event->m_frameData;
    }
}
} // sun::ui