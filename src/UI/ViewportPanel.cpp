//
// Created by Bellaedris on 26/01/2026.
//

#include "ViewportPanel.h"

#include "glm/gtx/matrix_decompose.hpp"
#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Components/Collider.h"
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
    // initial guizmo loading on viewport creation (after the scene has been deserialized)
    ActivateGuizmos(true);
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
                    // scene has reloaded, register the guizmos again
                    ActivateGuizmos(true);
                    m_state->temp.systems->m_camera->SetCursorVisible(true);
                }
                else
                {
                    // start the sim: do a backup of the scene, call OnPlay() on all components
                    // backup the scene and save the current editor scene
                    if (std::optional<std::string> activePath = m_scene->Serialize())
                        m_state->persistent.activeScenePath = activePath.value();
                    m_scene->OnPlay();
                    ActivateGuizmos(false);
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

        // draw transform guizmos
        lum::CameraSystem* camSys = m_state->temp.systems->m_camera;
        lum::Node3D* selected = m_state->temp.m_selectedNode;
        if (selected != nullptr && (camSys->CurrentMode() == lum::CameraSystem::Editor || camSys->CurrentMode() ==
             lum::CameraSystem::EditorInPlay))
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
            ImGuizmo::SetRect(pos.x, pos.y + iconSize * 2., availableSize.x, availableSize.y - iconSize);
            lum::comp::Transform* t = selected->GetTransform();
            bool manipulated = ImGuizmo::Manipulate(
                    glm::value_ptr(camSys->CameraData().viewMatrix),
                    glm::value_ptr(camSys->CameraData().projectionMatrix),
                    mCurrentGizmoOperation,
                    mCurrentGizmoMode,
                    glm::value_ptr(t->Model()),
                    nullptr,
                    nullptr);
            // since we directly edit the model matrix, we need to retrieve the individual components
            if (manipulated)
            {
                glm::quat rotation;
                glm::vec3 scale, position, skew;
                glm::vec4 perspective;
                glm::decompose(t->Model(), scale, rotation, position, skew, perspective);
                t->SetPosition(position);
                t->SetRotation(rotation);
                t->SetScale(scale);

                for (auto&& c : selected->Components())
                    c->RegisterGuizmo();
            }
        }

        // handle editor camera movements
        // if we're playing and press ESC, switch to EditorInPlay cam mode
        if (lum::InputManager::IsKeyDown(lum::KeyCode::lKeyEsc))
        {
            camSys->SetEditorInPlayMode();
            camSys->SetCursorVisible(true);
        }

        // if we're in play mode, we want to refresh guizmos each frame if something moves
        if (m_state->temp.isPlaying && camSys->CurrentMode() == lum::CameraSystem::EditorInPlay)
            ActivateGuizmos(true);

        if ((camSys->CurrentMode() == lum::CameraSystem::Editor || camSys->CurrentMode() ==
             lum::CameraSystem::EditorInPlay) && ImGui::IsWindowFocused())
        {
            if (lum::InputManager::IsMouseButtonDown(lum::MouseButton::lRightClick))
                m_state->temp.viewportCamera->ProcessMouseMovement(lum::InputManager::GetAxis());

            if (lum::InputManager::IsMouseButtonPressed(lum::MouseButton::lRightClick))
                camSys->SetCursorVisible(false);

            if (lum::InputManager::IsMouseButtonReleased(lum::MouseButton::lRightClick))
                camSys->SetCursorVisible(true);

            // get the input axes by reading pressed keys
            glm::vec3 dir(0, 0, 0);
            if(lum::InputManager::IsKeyDown(lum::KeyCode::lKeyW))
                dir += lum::VectorUtils::FORWARD;
            if(lum::InputManager::IsKeyDown(lum::KeyCode::lKeyA))
                dir -= lum::VectorUtils::RIGHT;
            if(lum::InputManager::IsKeyDown(lum::KeyCode::lKeyS))
                dir -= lum::VectorUtils::FORWARD;
            if(lum::InputManager::IsKeyDown(lum::KeyCode::lKeyD))
                dir += lum::VectorUtils::RIGHT;
            if(lum::InputManager::IsKeyDown(lum::KeyCode::lKeyQ))
                dir -= lum::VectorUtils::UP;
            if(lum::InputManager::IsKeyDown(lum::KeyCode::lKeyE))
                dir += lum::VectorUtils::UP;
            m_state->temp.viewportCamera->UpdatePosition(dir, m_state->temp.deltaTime);
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

void ViewportPanel::ActivateGuizmos(bool activate)
{
    m_scene->ForEachNode([&](lum::Node3D* node)
    {
        for (auto&& c : node->Components())
            activate ? c->RegisterGuizmo() : c->UnregisterGuizmo();
    });
}
} // sun::ui