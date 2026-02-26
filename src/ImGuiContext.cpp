//
// Created by Bellaedris on 26/01/2026.
//

#include "ImGuiContext.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/implot.h"
#include "imgui/IconsFontAwesome4.h"

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
}

void ImGuiContext::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // sun