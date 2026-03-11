//
// Created by Bellaedris on 27/01/2026.
//

#include "EditorState.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace sun {
void EditorState::Serialize()
{
    YAML::Node root;
    root["version"] = EDITOR_STATE_SERIALIZER_VERSION;
    root["activeScene"] = persistent.activeScenePath;

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

    persistent.activeScenePath = root["activeScene"].as<std::string>();
}
} // sun