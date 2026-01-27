//
// Created by Bellaedris on 27/01/2026.
//

#pragma once
#include "glm/vec2.hpp"

namespace sun
{
/**
 * \brief Stores informations on the editor. A part of these informations are temporary and are only useful for the current
 * session, the other part are persistent datas that are saved upon quitting to be loaded on the next session.
 */
struct EditorState
{
    struct
    {
        bool shouldNotifyViewportChange {false};
    } temp;

    struct
    {
        glm::ivec2 viewportSize;
    } persistent;
};
} // sun