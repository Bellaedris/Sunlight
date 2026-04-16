//
// Created by Bellaedris on 27/01/2026.
//

#include "EditorState.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include <Lumiere/Utils/YAMLUtils.h>

namespace sun {
void EditorState::Serialize()
{
    YAML::Node root;
    root["version"] = EDITOR_STATE_SERIALIZER_VERSION;
    root["activeScene"] = persistent.activeScenePath;
    root["cameraPosition"] = temp.viewportCamera->Position();
    root["cameraPitch"] = temp.viewportCamera->Pitch();
    root["cameraYaw"] = temp.viewportCamera->Yaw();

    std::ofstream out(EDITOR_CONFIG_PATH);
    out << root;
}

void EditorState::Deserialize()
{
    YAML::Node root;
    try
    {
        root = YAML::LoadFile(EDITOR_CONFIG_PATH);
    }
    catch (YAML::BadFile& e)
    {
        std::cerr << "EditorCfg is ill-formed or not created yet.\n";
        return;
    }
    if (root["version"].as<int>() != EDITOR_STATE_SERIALIZER_VERSION)
    {
        std::cerr << "The Editor configuration uses an older version. It should be recreated, or updated to match current specification\n";
        return;
    }

    persistent.activeScenePath = root["activeScene"].as<std::string>();
    temp.viewportCamera->SetPosition(root["cameraPosition"].as<glm::vec3>());
    temp.viewportCamera->SetPitchYaw(root["cameraPitch"].as<float>(), root["cameraYaw"].as<float>());
}
} // sun