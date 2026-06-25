---
--- Created by belle
--- DateTime: 09/05/2026 11:12
---

local mob = MaterialOverrideBlock()
local mr
local painted = false

function Start()
    mr = node:GetMeshComponent()
end

function Update()

end

function Interact(node, raycastRes)
    if (painted == false) then
        mob:SetVector("albedoColor", vec3(1, 0, 0))
        mr:SetMaterialOverrideBlock(mob)
        painted = true
    end
end