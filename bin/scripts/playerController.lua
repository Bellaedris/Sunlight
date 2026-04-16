---
--- Created by belle
--- DateTime: 30/03/2026 16:03
---

local speed = 10.
local rb
local camera

local isoPresets = {1. / 256., 1. / 64., 1. / 32., 1. / 8., 1. / 2., 1., 2., 4., 8., 16., 32.}
local currentIsoPreset = 6

local ISOUIButton

function Start()
    RenderSettings.cameraSensorIso = isoPresets[currentIsoPreset]
    SetCursorVisible(false)
    rb = node:GetRigidbodyComponent();
    Events.Subscribe("CameraRegistered", OnCameraRegistered)
    Events.Subscribe("UIButtonRegister", OnUIButtonRegister)
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
    if (RenderSettings.activePipeline == 1 and IsMouseButtonPressed(MouseButton.LeftClick)) then
        local ray = Ray(t.position, t.forward, 1000);
        local res = Physics:Raycast(ray);
        if (res ~= nil) then
            Message(res.node, "Interact", node, res)
        end
    end

    if RenderSettings.activePipeline == 0 then
        if IsMouseButtonPressed(MouseButton.LeftClick) then
            RenderSettings.accumulate = true;
        end

        if IsMouseButtonReleased(MouseButton.LeftClick) then
            RenderSettings.accumulate = false;
        end

        if IsKeyPressed(KeyCode.Q) then
            currentIsoPreset = math.max(1, currentIsoPreset - 1)
            RenderSettings.cameraSensorIso = isoPresets[currentIsoPreset]
            Message(ISOUIButton, "UIUpdate", currentIsoPreset - 6)
        end

        if IsKeyPressed(KeyCode.E) then
            currentIsoPreset = math.min(#isoPresets, currentIsoPreset + 1)
            RenderSettings.cameraSensorIso = isoPresets[currentIsoPreset]
            Message(ISOUIButton, "UIUpdate", currentIsoPreset - 6)
        end
    end

    if IsMouseButtonPressed(MouseButton.RightClick) then
        local pipelineId = RenderSettings.activePipeline
        RenderSettings.activePipeline = (RenderSettings.activePipeline + 1) % 2
    end

end

function OnCameraRegistered(node)
    camera = node
end

function OnUIButtonRegister(node)
    if node.name == "ISO handle" then
        ISOUIButton = node
    end
end