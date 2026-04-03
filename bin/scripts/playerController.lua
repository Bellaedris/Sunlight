---
--- Created by belle
--- DateTime: 30/03/2026 16:03
---

local speed = 10.
local rb
local camera

function Start()
    SetCursorVisible(false)
    rb = node:GetRigidbodyComponent();
    Events.Subscribe("CameraRegistered", OnCameraRegistered)
end

function Update(dt)
    if camera == nil then
        return
    end
    -- movements
    local t = camera.transform
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
    -- remove movement in Y axis!!
    movement.y = 0
    rb:AddForce(movement * speed)

    -- interactions
    if (IsMouseButtonPressed(MouseButton.LeftClick)) then
        local ray = Ray(node.transform.position, t.forward, 1000);
        local res = Physics:Raycast(ray);
        if (res ~= nil) then
            -- Events.Emit("RaycastHitBroadcast")
            Message(res.node, "Interact", node, res)
        end
    end
end

function OnCameraRegistered(node)
    camera = node
end