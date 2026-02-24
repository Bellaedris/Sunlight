//
// Created by Bellaedris on 27/01/2026.
//

#pragma once
#include "Lumiere/Node3D.h"
#include "glm/vec2.hpp"
#include "stduuid/uuid.h"

namespace sun
{
/**
 * \brief Stores information on the editor. A part of this information is temporary and are only useful for the current
 * session, the other part are persistent datas that are saved upon quitting to be loaded on the next session.
 */
struct EditorState
{
    struct
    {
        lum::Node3D* m_selectedNode;
        bool shouldNotifyViewportChange {false};
    } temp;

    struct
    {
        glm::ivec2 viewportSize;
    } persistent;
};
} // sun