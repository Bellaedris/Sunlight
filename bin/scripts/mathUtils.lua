---
--- Created by belle
--- DateTime: 09/05/2026 10:52
---

function Clamp(number, min, max)
    res = number
    if number < min then
        res = min
    elseif number > max then
        res = max
    end
    return res
end