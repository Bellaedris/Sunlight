---
--- Created by belle
--- DateTime: 30/03/2026 16:03
---

local speed = 1.

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
    SetCursorVisible(false)
end

local rotX = 0
local rotY = 0

function Update(dt)
    local t = node.transform
    -- movements
    local movement = vec3(0, 0, 0)
    if (IsKeyDown(KeyCode.W)) then
        movement =  t.forward * speed * dt
    end
    if (IsKeyDown(KeyCode.S)) then
        movement = movement - t.forward * speed * dt
    end
    if (IsKeyDown(KeyCode.A)) then
        movement = movement + t.right * -speed * dt
    end
    if (IsKeyDown(KeyCode.D)) then
        movement = movement + t.right * speed * dt
    end
    -- remove movement in Y axis!!
    movement.y = 0
    t:Translate(movement)

    -- camera rotation
    local axis = GetAxis()
    rotX = rotX + axis.y
    rotX = Clamp(rotX, -90., 90.)
    rotY = rotY - axis.x
    node.transform.euler = vec3(rotX, rotY, 0)

    -- interactions
    if(IsMouseButtonPressed(MouseButton.LeftClick)) then
        local ray = Ray(node.transform.position, node.transform.forward, 1000);
        local res = Physics:Raycast(ray);
        if (res ~= nil) then
            -- Events.Emit("RaycastHitBroadcast")
            Message(res.node, "RaycastHit", res)
        end
    end
end