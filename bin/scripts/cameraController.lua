---
--- Created by belle
--- DateTime: 03/04/2026 12:37
---

local rotX = 0
local rotY = 0

function Clamp(number, min, max)
    res = number
    if number < min then
        res = min
    elseif number > max then
        res = max
    end
    return res
end

function Start()
    Events.Emit("CameraRegistered", node)
end

function Update()
    -- camera rotation
    local axis = GetAxis()
    rotX = rotX + axis.y
    rotX = Clamp(rotX, -90., 90.)
    rotY = rotY - axis.x
    node.transform.euler = vec3(rotX, rotY, 0)
end