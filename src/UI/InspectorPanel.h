//
// Created by belle on 24/02/2026.
//

#pragma once
#include "IEditorPanel.h"
#include "../EditorState.h"
#include "Lumiere/Components/MeshRenderer.h"
#include "Lumiere/Components/UIElement.h"
#include "imgui/IconsFontAwesome4.h"
#include "imgui/imfilebrowser.h"

namespace sun::ui
{
class InspectorPanel : public IEditorPanel
{
private:
    std::shared_ptr<EditorState> m_state;
    int m_selectedComponentType {0};
    ImGui::FileBrowser m_fileBrowser;
    ImGui::FileBrowser m_scriptBrowser;

    void DrawTransformInspector(lum::comp::Transform* transform, ImGuiTreeNodeFlags flags);
    void DrawMeshDetails(lum::comp::MeshRenderer* renderer);
    void DrawUIDetails(lum::comp::UIElement *ui);
    void TransformSlider(const char* name, glm::vec3 vector, float defaultValue, std::function<void(const glm::vec3&)> updateVector);

    template<typename T>
    void ComponentInspectorNode(lum::Node3D* node, const std::string& name, const std::string& icon, ImGuiTreeNodeFlags flags,std::function<void(T*)> callback);

    template <typename T>
    void ComponentCreationButton(lum::Node3D* node, const char* name, const char* icon);

public:
    InspectorPanel(const std::shared_ptr<EditorState>& state);

    void Render() override;
};

template<typename T>
void InspectorPanel::ComponentInspectorNode(lum::Node3D* node, const std::string& name, const std::string& icon, ImGuiTreeNodeFlags flags, std::function<void(T*)> callback)
{
    if (std::optional<T*> c = node->GetComponent<T>())
    {
        T* component = c.value();

        ImGui::PushID(name.c_str());
        bool open = ImGui::TreeNodeEx((icon + " " + name).c_str(), flags);
        float elementHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - elementHeight);
        if (ImGui::Button(ICON_FA_TRASH))
        {
            node->RemoveComponent(component);
        }
        if (open)
        {
            callback(component);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

template<typename T>
void InspectorPanel::ComponentCreationButton(lum::Node3D* node, const char *name, const char* icon)
{
    if (node->HasComponent<T>() == false)
        if (ImGui::Selectable((icon + std::string(" ") + name).c_str(), false))
            node->AddComponent<T>();
}
} // sun::ui