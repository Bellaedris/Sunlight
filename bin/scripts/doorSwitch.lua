---
--- Created by belle
--- DateTime: 09/04/2026 10:05
---

-- called when the player clicks the button, sends the button ID to the door
function Interact(sender, hitInfo)
    Events.Emit("OpenSignal", tonumber(node.name))
end