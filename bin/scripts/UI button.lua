---
--- Created by belle
--- DateTime: 15/04/2026 10:50
---

local initialPosition
local offsetPerButton = vec3(.17, 0, 0)

function Start()
    Events.Emit("UIButtonRegister", node)
    initialPosition = vec3(node.transform.localPosition)
end

function Update()

end

function UIUpdate(ISOIndex)
    node.transform.localPosition = initialPosition + ISOIndex * offsetPerButton
end