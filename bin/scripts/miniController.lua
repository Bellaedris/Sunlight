---
--- Created by belle
--- DateTime: 09/05/2026 10:43
---

local mathUtils = require "mathUtils"

local moveSpeed = 5.
local viewSpeed = 1.
local rotX = 0
local rotY = 0

function Start()
    SetCursorVisible(false)
end

function Update(dt)
    local t = node.transform
    local movement = vec3(0, 0, 0)
    if (IsKeyDown(KeyCode.W)) then
        movement = t.forward
    end
    if (IsKeyDown(KeyCode.S)) then
        movement = movement - t.forward
    end
    if (IsKeyDown(KeyCode.A)) then
        movement = movement - t.right
    end
    if (IsKeyDown(KeyCode.D)) then
        movement = movement + t.right
    end
    movement.y = 0
    t:Translate(movement * moveSpeed * dt)

    -- rotation now
    local axis = GetAxis() -- retrieve mouse axis
    rotX = rotX + axis.y
    rotX = Clamp(rotX, -90, 90)
    rotY = rotY - axis.x
    node.transform.euler = vec3(rotX, rotY, 0) * viewSpeed

    if (IsMouseButtonPressed(MouseButton.LeftClick)) then
        local ray = Ray(t.position, t.forward, 1000)
        local res = Physics:Raycast(ray)
        if(res ~= nil) then
            Message(res.node, "Interact", node, res)
        end
    end
end