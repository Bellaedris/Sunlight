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
    -- empty
end

local rotX = 0
local rotY = 0

function Update(dt)
    -- movements
    local movement = vec3(0, 0, 0)
    if (IsKeyDown(KeyCode.W)) then
        movement.x = speed * dt
    end
    if (IsKeyDown(KeyCode.A)) then
        movement.z = -speed * dt
    end
    if (IsKeyDown(KeyCode.S)) then
        movement.x = movement.x - speed * dt
    end
    if (IsKeyDown(KeyCode.D)) then
        movement.z = movement.z + speed * dt
    end
    node.transform:Translate(movement)

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
        if(res ~= nil) then
            print(res.node.name)
        end
    end
end