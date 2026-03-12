//
// Created by Bellaedris on 26/01/2026.
//

#include "ImGuiContext.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/implot.h"
#include "imgui/IconsFontAwesome4.h"
#include "imgui/ImGuizmo.h"

#include <filesystem>
#include <iostream>

namespace sun {
ImGuiContext::ImGuiContext(const std::shared_ptr<lum::Window>& window)
    : m_window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    io.Fonts->AddFontDefaultVector();
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    io.Fonts->AddFontFromFileTTF("config/fontawesome-webfont.ttf", 13.0f, &config);

    if (std::filesystem::exists(IMGUI_INI_LOCATION) == false)
    {
        std::cerr << "Couldn't find imgui config file at " << IMGUI_INI_LOCATION << "\n";
    }
    else
    {
        io.IniFilename = nullptr;

        ImGui::LoadIniSettingsFromDisk(IMGUI_INI_LOCATION);
    }

    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init();

    // load custom style
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.03f, 0.03f, 0.03f, 0.86f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.14f, 0.14f, 0.15f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.14f, 0.14f, 0.15f, 0.86f);
    colors[ImGuiCol_Border]                 = ImVec4(0.52f, 0.52f, 0.52f, 0.46f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.15f, 0.15f, 0.15f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.93f, 0.64f, 0.46f, 0.62f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.46f, 0.46f, 0.46f, 0.62f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 0.47f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.16f, 0.26f, 0.47f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.27f, 0.27f, 0.28f, 0.74f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.28f, 0.28f, 0.27f, 0.55f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.51f, 0.47f, 0.40f, 0.47f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.99f, 0.64f, 0.42f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.99f, 0.53f, 0.38f, 0.84f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.88f, 0.88f, 0.88f, 0.76f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.68f, 0.68f, 0.68f, 0.57f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.29f, 0.29f, 0.29f, 0.77f);
    colors[ImGuiCol_Button]                 = ImVec4(0.33f, 0.34f, 0.35f, 0.45f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.73f, 0.46f, 0.30f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.57f, 0.36f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.46f, 0.47f, 0.50f, 0.49f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.73f, 0.46f, 0.29f, 0.62f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.57f, 0.36f, 0.23f, 0.62f);
    colors[ImGuiCol_Separator]              = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.98f, 0.98f, 0.98f, 0.78f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.98f, 0.98f, 0.98f, 0.55f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.98f, 0.98f, 0.98f, 0.83f);
    colors[ImGuiCol_InputTextCursor]        = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.93f, 0.64f, 0.46f, 0.74f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.68f, 0.47f, 0.34f, 0.79f);
    colors[ImGuiCol_TabSelected]            = ImVec4(0.57f, 0.36f, 0.23f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.93f, 0.64f, 0.46f, 1.00f);
    colors[ImGuiCol_TabDimmed]              = ImVec4(0.14f, 0.12f, 0.07f, 0.89f);
    colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.47f, 0.29f, 0.19f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.49f, 0.49f, 0.49f, 0.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.93f, 0.64f, 0.46f, 0.64f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.35f, 0.56f, 0.98f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.01f, 0.30f, 0.88f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.01f, 0.34f, 0.98f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.18f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.30f, 0.32f, 0.34f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.22f, 0.23f, 0.24f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.98f, 0.98f, 0.98f, 0.06f);
    colors[ImGuiCol_TextLink]               = ImVec4(0.26f, 0.50f, 0.96f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.18f, 0.39f, 0.78f, 0.83f);
    colors[ImGuiCol_TreeLines]              = ImVec4(0.42f, 0.45f, 0.49f, 0.46f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.99f, 0.73f, 0.39f, 0.83f);
    colors[ImGuiCol_DragDropTargetBg]       = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_UnsavedMarker]          = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
    colors[ImGuiCol_NavCursor]              = ImVec4(0.26f, 0.50f, 0.96f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.98f, 0.98f, 0.98f, 0.64f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.78f, 0.78f, 0.78f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.78f, 0.78f, 0.78f, 0.35f);
}

ImGuiContext::~ImGuiContext()
{
    if (std::filesystem::exists(IMGUI_INI_LOCATION) == false)
    {
        std::cerr << "Couldn't find imgui config file at " << IMGUI_INI_LOCATION << "\n";
    }
    else
    {
        ImGui::SaveIniSettingsToDisk(IMGUI_INI_LOCATION);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void ImGuiContext::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void ImGuiContext::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // sun