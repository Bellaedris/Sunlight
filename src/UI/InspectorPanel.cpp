//
// Created by belle on 24/02/2026.
//

#include "InspectorPanel.h"

#include "Lumiere/ResourcesManager.h"
#include "Lumiere/Components/BoxCollider.h"
#include "Lumiere/Components/Light.h"
#include "Lumiere/Components/MeshRenderer.h"
#include "Lumiere/Components/Rigidbody.h"
#include "Lumiere/Components/Script.h"
#include "Lumiere/Components/SphereCollider.h"
#include "Lumiere/Components/UIElement.h"
#include "imgui/IconsFontAwesome4.h"
#include "imgui/ImGuizmo.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"

namespace sun::ui
{
InspectorPanel::InspectorPanel(const std::shared_ptr<EditorState> &state)
    : m_state(state)
{
    m_fileBrowser.SetTitle("Mesh selection");
    m_fileBrowser.SetTypeFilters({".gltf", ".glb", ".obj", ".fbx"});

    m_scriptBrowser.SetTitle("Script selection");
    m_scriptBrowser.SetTypeFilters({".lua"});
}

void InspectorPanel::Render()
{
    ImGui::Begin("Inspector");
    {
        if (m_state->temp.m_selectedNode != nullptr)
        {
            lum::Node3D* node = m_state->temp.m_selectedNode;

            ImGui::InputText("Name", &node->Name());

            // component sub region flags
            static ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth |
                                              ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap;
            // Transform
            DrawTransformInspector(node->GetTransform(), flags);

            // size of a single font element, to add trash icon
            float elementHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

            // Components
            // Mesh Renderer
            ComponentInspectorNode<lum::comp::MeshRenderer>(
                node,
                "Mesh Renderer",
                ICON_FA_CUBE,
                flags,
                [&](lum::comp::MeshRenderer* renderer)
                {
                    if (ImGui::Button("Load mesh from file"))
                    {
                        // load a mesh from file/resourcesManager
                        m_fileBrowser.Open();
                    }

                    m_fileBrowser.Display();
                    if (m_fileBrowser.HasSelected())
                    {
                        renderer->SetMesh(m_fileBrowser.GetSelected().string());
                        m_fileBrowser.ClearSelected();
                    }
                    ImGui::PushID("Asset selection");
                    if (ImGui::BeginCombo("", "Asset selection"))
                    {
                        std::vector<std::pair<std::string, std::string>> resources = lum::ResourcesManager::Instance()->MeshNames();
                        for (const auto& resource : resources)
                        {
                            bool selected = false;
                            if (renderer->Mesh() != nullptr && renderer->Mesh()->Name() == resource.second)
                                selected = true;
                            if (ImGui::Selectable(resource.second.c_str(), selected))
                                renderer->SetMesh(resource.first);
                            if (selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::PopID();
                    if (renderer->Mesh() != nullptr)
                    {
                        DrawMeshDetails(renderer);
                    }
                }
            );

            // UIElement
            ComponentInspectorNode<lum::comp::UIElement>(
                node,
                "UI Element",
                ICON_FA_WINDOW_MAXIMIZE,
                flags,
                [&](lum::comp::UIElement* ui)
                {
                    if (ImGui::Button("Load UI Mesh from file"))
                    {
                        // load a mesh from file/resourcesManager
                        m_fileBrowser.Open();
                    }

                    m_fileBrowser.Display();
                    if (m_fileBrowser.HasSelected())
                    {
                        ui->SetMesh(m_fileBrowser.GetSelected().string());
                        m_fileBrowser.ClearSelected();
                    }
                    ImGui::PushID("Asset selection");
                    if (ImGui::BeginCombo("", "Asset selection"))
                    {
                        std::vector<std::pair<std::string, std::string>> resources = lum::ResourcesManager::Instance()->MeshNames();
                        for (const auto& resource : resources)
                        {
                            bool selected = false;
                            if (ui->Mesh() != nullptr && ui->Mesh()->Name() == resource.second)
                                selected = true;
                            if (ImGui::Selectable(resource.second.c_str(), selected))
                                ui->SetMesh(resource.first);
                            if (selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::PopID();
                    if (ui->Mesh() != nullptr)
                    {
                        DrawUIDetails(ui);
                    }
                }
            );

            // Script
            ComponentInspectorNode<lum::comp::Script>(
                node,
                "Script",
                ICON_FA_FILE_CODE_O,
                flags,
                [&](lum::comp::Script* script)
                {
                    if (script->Path().empty() == false)
                    {
                        ImGui::Text("Script : %s", script->Name().c_str());
                        if (ImGui::Button("Reload Script"))
                            script->LoadScript();
                    }
                    if (ImGui::Button("Select a script"))
                    {
                        // load a mesh from file/resourcesManager
                        m_scriptBrowser.Open();
                    }
                    m_scriptBrowser.Display();
                    if (m_scriptBrowser.HasSelected())
                    {
                        script->SetScriptPath(m_scriptBrowser.GetSelected().string());
                        m_scriptBrowser.ClearSelected();
                    }
                }
            );

            // Light
            ComponentInspectorNode<lum::comp::Light>(
                node,
                "Light",
                ICON_FA_LIGHTBULB_O,
                flags,
                [&](lum::comp::Light* light)
                {
                    ImGui::Combo("Light type", &light->Type(), lum::comp::Light::LIGHT_TYPES, lum::comp::Light::LIGHT_TYPE_COUNT);

                    ImGui::ColorPicker3("Tint", glm::value_ptr(light->Color()));
                    ImGui::DragFloat("Intensity", &light->Intensity(), 0.1f);

                    // type dependant editor: 0 is directional, 1 point, 2 spot
                    if (light->Type() == 1)
                    {
                        ImGui::DragFloat("Range", &light->PointRange(), 1.f);
                    }
                }
            );

            // Rigidbody
            ComponentInspectorNode<lum::comp::Rigidbody>(
                    node,
                    "Rigidbody",
                    ICON_FA_SUN_O,
                    flags,
                    [&](lum::comp::Rigidbody *rb)
            {
                if (ImGui::BeginCombo("Body type", rb->SelectedMotionType()))
                {
                    for (int n = 0; n < lum::comp::Rigidbody::DISPLAYABLE_MOTION_TYPES.size(); n++)
                    {
                        const bool is_selected = (rb->SelectedMotionTypeIndex() == n);
                        if (ImGui::Selectable(lum::comp::Rigidbody::DISPLAYABLE_MOTION_TYPES[n], is_selected))
                            rb->SetSelectedMotionType(n);

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::DragFloat("Mass", &rb->Mass(), 1.f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.6f");
                ImGui::DragFloat("Linear damping", &rb->LinearDamping(), .1f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.6f");
                ImGui::DragFloat("Angular damping", &rb->AngularDamping(), .1f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.6f");
            });

            // Colliders
            ComponentInspectorNode<lum::comp::BoxCollider>(
                    node,
                    "Box Collider",
                    ICON_FA_CUBE,
                    flags,
                    [&](lum::comp::BoxCollider *bc)
            {
                if (ImGui::DragFloat3(
                        "Size",
                        glm::value_ptr(bc->Size()),
                        1.f,
                        std::numeric_limits<float>::lowest(),
                        std::numeric_limits<float>::max(),
                        "%.6f"))
                {
                    bc->RegisterGuizmo();
                };
            });

            ComponentInspectorNode<lum::comp::SphereCollider>(
                    node,
                    "Sphere Collider",
                    ICON_FA_CIRCLE,
                    flags,
                    [&](lum::comp::SphereCollider *sc)
            {
                if (ImGui::DragFloat(
                        "Radius",
                        &sc->Radius(),
                        1.f,
                        std::numeric_limits<float>::lowest(),
                        std::numeric_limits<float>::max(),
                        "%.6f"))
                {
                    sc->RegisterGuizmo();
                }
            });

            ComponentInspectorNode<lum::comp::Camera>(
                    node,
                    "Camera",
                    ICON_FA_VIDEO_CAMERA,
                    flags,
                    [&](lum::comp::Camera *cam)
            {
                if (ImGui::BeginCombo("Projection type", cam->CameraType().c_str()))
                {
                    static const char* types[] = {"Perspective", "Orthographic"};
                    for (int n = 0; n < 2; n++)
                    {
                        const bool is_selected = (cam->CameraType().c_str() == types[n]);
                        if (ImGui::Selectable(types[n], is_selected))
                            cam->SetCameraType(n == 0);

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                // perspective specific settings
                if (cam->IsPerspective())
                    ImGui::DragFloat("Field of View", &cam->Fov(), 10.f, 0.f, 360.f);
                else
                    ImGui::DragFloat("View size", &cam->OrthoSize(), 1.f, 0.f, 1000.f);

                ImGui::DragFloat("Z near", &cam->ZNear(), .1f, 0.f, 1000.f);
                ImGui::DragFloat("Z far", &cam->ZFar(), .1f, 0.f, 1000.f);
                ImGui::InputFloat2("Viewport min", glm::value_ptr(cam->ViewportMin()));
                ImGui::InputFloat2("Viewport max", glm::value_ptr(cam->ViewportMax()));
            });

            // new component creation
            float buttonWidth = ImGui::GetContentRegionAvail().x * (2.f / 3.f);
            float size = ImGui::GetStyle().FramePadding.x * 2.0f;
            ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - buttonWidth) * .5f);
            if (ImGui::Button("AddComponent", {buttonWidth, 0}))
            {
                ImGui::OpenPopup("ComponentSelection");
            }

            if (ImGui::IsPopupOpen("ComponentSelection"))
            {
                ImVec2 addButtonPos = ImGui::GetItemRectMin();
                ImVec2 addButtonSize = ImGui::GetItemRectSize();
                ImGui::SetNextWindowSizeConstraints(
                    ImVec2(.0f, .0f),
                    ImVec2(FLT_MAX, 300.0f) // if the popup contains too many components, adds a scrollbar
                );
                ImGui::SetNextWindowPos(ImVec2(addButtonPos.x, addButtonPos.y + addButtonSize.y));
                ImGui::SetNextWindowSize(ImVec2(buttonWidth, 0.0f));
            }

            if (ImGui::BeginPopup("ComponentSelection"))
            {
                ComponentCreationButton<lum::comp::MeshRenderer>(node, "Mesh Renderer", ICON_FA_CUBE);
                ComponentCreationButton<lum::comp::Script>(node, "Lua Script", ICON_FA_FILE_CODE_O);
                ComponentCreationButton<lum::comp::Light>(node, "Light Source", ICON_FA_LIGHTBULB_O);
                ComponentCreationButton<lum::comp::Rigidbody>(node, "Rigidbody", ICON_FA_SUN_O);
                ComponentCreationButton<lum::comp::BoxCollider>(node, "Box Collider", ICON_FA_CUBE);
                ComponentCreationButton<lum::comp::SphereCollider>(node, "Sphere Collider", ICON_FA_CIRCLE);
                ComponentCreationButton<lum::comp::Camera>(node, "Camera", ICON_FA_VIDEO_CAMERA);
                ComponentCreationButton<lum::comp::UIElement>(node, "UI Element", ICON_FA_WINDOW_MAXIMIZE);
                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();
}

void InspectorPanel::DrawTransformInspector(lum::comp::Transform* transform, ImGuiTreeNodeFlags flags)
{
    if (ImGui::TreeNodeEx(ICON_FA_ARROWS " Transform", flags))
    {
        ImGui::BeginTable("Transform", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX);

        ImGui::TableNextRow();

        bool shouldReloadGuizmos = false;
        TransformSlider("Position", transform->LocalPosition(), .0f, [&](const glm::vec3& vector)
        {
            transform->SetLocalPosition(vector);
            shouldReloadGuizmos = true;
        });

        ImGui::TableNextRow();
        TransformSlider("Rotation", transform->EulerAngles(), .0f, [&](const glm::vec3& vector)
        {
            transform->SetEulerAngles(vector);
            shouldReloadGuizmos = true;
        });

        ImGui::TableNextRow();
        TransformSlider("Scale", transform->LocalScale(), 1.f, [&](const glm::vec3& vector)
        {
            transform->SetLocalScale(vector);
            shouldReloadGuizmos = true;
        });
        ImGui::EndTable();

        ImGui::TreePop();
        if (shouldReloadGuizmos)
            for (auto&& c : transform->Node()->Components())
                c->RegisterGuizmo();
    }
}

void InspectorPanel::DrawMeshDetails(lum::comp::MeshRenderer *renderer)
{
    ImGui::Text("Current Mesh: %s", renderer->Mesh()->Name().c_str());
    if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_DrawLinesFull))
    {
        std::vector<lum::gfx::MaterialPtr>& materials = renderer->Materials();
        for (int i = 0; i < materials.size(); i++)
        {
            ImGui::PushID(i);
            if (ImGui::TreeNodeEx(materials[i]->Name().c_str()))
            {
                materials[i]->DrawEditor();
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}

void InspectorPanel::DrawUIDetails(lum::comp::UIElement *ui)
{
    ImGui::Text("Current Mesh: %s", ui->Mesh()->Name().c_str());
    if (ImGui::TreeNodeEx("Submeshes", ImGuiTreeNodeFlags_DrawLinesFull))
    {
        std::vector<lum::gfx::SubMesh>& submeshes = ui->Mesh()->Primitives();
        for (auto& submesh : submeshes)
        {
            if (ImGui::TreeNodeEx(submesh.Name().c_str(), ImGuiTreeNodeFlags_DrawLinesFull))
            {
                lum::ResourcesManager::Instance()->GetMaterial(submesh.DefaultMat())->get()->DrawEditor();
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}

void InspectorPanel::TransformSlider(
        const char *name,
        glm::vec3 vector,
        float defaultValue,
        std::function<void(const glm::vec3 &)> updateVector)
{
    ImGui::AlignTextToFramePadding();

    ImGui::TableNextColumn();
    ImGui::Text("%s", name);

    ImGui::TableNextColumn();
    ImGui::PushMultiItemsWidths(3, ImGui::GetContentRegionAvail().x - 30);

    ImGui::PushID(name);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    //ImGui::PushStyleColor(ImGuiCol_ButtonActive, EditorCol_Secondary2);
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
        {
            if (ImGui::Button("X")) {
                updateVector(glm::vec3(defaultValue, vector.y, vector.z));
            }
            ImGui::SameLine();
            if (ImGui::DragFloat("##X", &vector.x, 0.01f, 0, 0, "%.6f"))
            {
                updateVector(vector);
            }
            ImGui::SameLine();
        }
        ImGui::PopStyleColor(2); // Button, ButtonHovered
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 1, 0, 1));
        {
            if (ImGui::Button("Y")) {
                updateVector(glm::vec3(vector.x, defaultValue, vector.z));
            }
            ImGui::SameLine();
            if (ImGui::DragFloat("##Y", &vector.y, 0.01f, 0, 0, "%.6f"))
            {
                updateVector(vector);
            }
            ImGui::SameLine();
        }
        ImGui::PopStyleColor(2); // Button, ButtonHovered
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 1, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 1, 1));
        {
            if (ImGui::Button("Z"))
            {
                updateVector(glm::vec3(vector.x, vector.y, defaultValue));
            }
            ImGui::SameLine();
            if (ImGui::DragFloat("##Z", &vector.z, 0.01f, 0, 0, "%.6f"))
            {
                updateVector(vector);
            }
        }
        ImGui::PopStyleColor(2); // Button, ButtonHovered
    }

    ImGui::PopStyleVar();
    ImGui::PopID();
}
} // sun::ui