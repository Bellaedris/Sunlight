---
--- Created by belle
--- DateTime: 13/04/2026 11:05
---

local movementType
local initialPosition
local moveAmplitude = 2.
local moveSpeed = 1.

local elapsed = 0
local t

local sqrt2 = math.sqrt(2)
local mob = MaterialOverrideBlock()

function Start()
    movementType = node.name
    t = node.transform
    initialPosition = vec3(t.position)
    local mr = node:GetMeshComponent()

    mob:SetVector("albedoColor", vec3(1., 0., 0.))
    mob:SetVector("emissionColor", vec3(1, 0, 0))
    mob:SetFloat("emissionStrength", 250.)
    mr:SetMaterialOverrideBlock(mob)
end

function Update(dt)
    if movementType == "horizontal" then
        MoveHorizontal()
    elseif movementType == "vertical" then
        MoveVertical()
    elseif movementType == "diagonal" then
        MoveDiagonal()
    elseif movementType == "circle" then
        MoveCircle()
    elseif movementType == "halfCircle" then
        MoveHalfCircle()
    end
    elapsed = elapsed + dt
end

function MoveHorizontal()
    t.position = initialPosition + vec3(0, 0, math.cos(elapsed * moveSpeed) * 0.7) * moveAmplitude
end

function MoveVertical()
    t.position = initialPosition + vec3(0, math.cos(elapsed * moveSpeed * 1.3), 0) * moveAmplitude
end

function MoveDiagonal()
    t.position = initialPosition + vec3(0, math.cos(elapsed * moveSpeed), math.cos(elapsed * moveSpeed)) * moveAmplitude
end

function MoveHalfCircle()
    t.position = initialPosition + vec3(0, math.cos(elapsed * moveSpeed), math.abs(math.sin(elapsed * moveSpeed))) * moveAmplitude
end

function MoveCircle()
    t.position = initialPosition + vec3(0, math.cos(elapsed * moveSpeed), math.sin(elapsed * moveSpeed)) * moveAmplitude
end