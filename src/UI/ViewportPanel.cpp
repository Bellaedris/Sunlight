//
// Created by Bellaedris on 26/01/2026.
//

#include "ViewportPanel.h"

#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Events/RenderEvents.h"
#include "Lumiere/Renderer/RenderPipeline.h"
#include "Lumiere/Renderer/Passes/ShadeNPR.h"
#include "imgui/ImGuizmo.h"
#include "imgui/imgui_internal.h"
#include "imgui/IconsFontAwesome4.h"

namespace sun::ui {
ViewportPanel::ViewportPanel(const std::shared_ptr<EditorState>& editorState, const std::shared_ptr<lum::rdr::SceneDesc>& scene)
    : m_state(editorState)
    , m_scene(scene)
{

}

void ViewportPanel::Render()
{
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        ImVec2 availableSize = ImGui::GetContentRegionAvail();

        float iconSize = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        // play mode buttons
        ImGui::BeginChild("Toolbox", ImVec2(availableSize.x, iconSize));
        {
            // play/pause
            if (ImGui::Button(m_state->temp.isPlaying ? ICON_FA_STOP : ICON_FA_PLAY))
            {
                if (m_state->temp.isPlaying)
                {
                    // call OnStop() for all components
                    m_scene->OnStop();
                    // restore the original scene without saving what has moved
                    m_scene->Deserialize(m_state->persistent.activeScenePath);
                    // pointers are invalidated after scene reload
                    m_state->temp.m_selectedNode = nullptr;
                }
                else
                {
                    // start the sim: do a backup of the scene, call OnPlay() on all components
                    // backup the scene
                    m_scene->Serialize();
                    m_scene->OnPlay();
                }
                m_state->temp.isPlaying = !m_state->temp.isPlaying;
            }
        }
        ImGui::EndChild();

        if (m_lastViewportSize.x != availableSize.x || m_lastViewportSize.y != availableSize.y)
        {
            // initial resizing: the 1st render call has lastViewportSize == 0, but the available size seems to ignore the dockspace...
            // we have to wait the second frame to resize, which is the first non-zero lastViewportSize.
            if (m_lastViewportSize.x != 0 && m_initialized == false)
            {
                m_state->temp.shouldNotifyViewportChange = true;
                m_state->temp.viewportSize = {availableSize.x, availableSize.y};
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
            m_state->temp.viewportSize = {availableSize.x, availableSize.y};
            m_isResizing = false;
        }

        if (m_windowResized)
        {
            m_state->temp.shouldNotifyViewportChange = true;
            m_state->temp.viewportSize = {availableSize.x, availableSize.y};
            m_windowResized = false;
        }
        m_lastViewportSize = availableSize;
        glm::ivec2 size = m_lastRenderedFrame->Size();
        // we change the UVs because of OpenGL's flipped Y axis
        ImGui::Image(m_lastRenderedFrame->Handle(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

        // draw guizmos
        lum::Node3D* selected = m_state->temp.m_selectedNode;
        if (selected != nullptr)
        {
            static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
            static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
            if (ImGui::IsKeyPressed(ImGuiKey_T))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            if (ImGui::IsKeyPressed(ImGuiKey_E))
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            if (ImGui::IsKeyPressed(ImGuiKey_R))
                mCurrentGizmoOperation = ImGuizmo::SCALE;

            ImVec2 pos = ImGui::GetWindowPos();
            ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);
            ImGuizmo::SetRect(pos.x, pos.y, m_lastViewportSize.x, m_lastViewportSize.y);
            lum::comp::Transform& t = selected->GetTransform();
            bool manipulated = ImGuizmo::Manipulate(
                    glm::value_ptr(m_state->temp.viewportCamera->View()),
                    glm::value_ptr(m_state->temp.viewportCamera->Projection()),
                    mCurrentGizmoOperation,
                    mCurrentGizmoMode,
                    glm::value_ptr(t.Model()),
                    nullptr,
                    nullptr);
            // since we directly edit the model matrix, we need to retrieve the individual components
            if (manipulated)
            {
                float translation[3];
                float rotation[3];
                float scale[3];
                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(t.Model()), translation, rotation, scale);
                t.SetLocalPosition(glm::vec3(translation[0], translation[1], translation[2]));
                t.SetLocalRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));
                t.SetLocalScale(glm::vec3(scale[0], scale[1], scale[2]));
            }
        }
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

    if (e->Type() == lum::evt::WindowResized)
    {
        m_windowResized = true;
    }
}
} // sun::ui