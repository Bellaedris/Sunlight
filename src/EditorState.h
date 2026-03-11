//
// Created by Bellaedris on 27/01/2026.
//

#pragma once
#include "Lumiere/Node3D.h"
#include "glm/vec2.hpp"
#include "Lumiere/Renderer/Camera.h"

namespace sun
{
/**
 * \brief Stores information on the editor. A part of this information is temporary and are only useful for the current
 * session, the other part are persistent datas that are saved upon quitting to be loaded on the next session.
 */
struct EditorState
{
    #pragma region Constants
    constexpr static int EDITOR_STATE_SERIALIZER_VERSION = 1;
    constexpr static const char* EDITOR_CONFIG_PATH = "config/EditorCfg.yaml";
    #pragma endregion // Constants

    struct
    {
        lum::Node3D* m_selectedNode;
        bool shouldNotifyViewportChange {false};
        // TODO the camera should be stored in the persistent storage, but since we use a weird camera model that does
        // not depend on gameplay/editor for now, i'll just leave it here
        lum::rdr::Camera* viewportCamera;
        glm::ivec2 viewportSize;
    } temp;

    struct
    {
        /** \brief relative path of the active scene */
        std::string activeScenePath;
    } persistent;

    void Serialize();
    void Deserialize();
};
} // sun