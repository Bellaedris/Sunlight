---
--- Created by belle
--- DateTime: 09/04/2026 09:57
---

local timeToOpen = 2.
local shouldOpen = false
local isOpen = false
local timeSinceOpen = 0.
local doorInitialPosition
local doorDestination = vec3(-15., 0., 0.)
local elapsed = 0.
local codeSize = 2
local code = { 1, 2 }
local codeState = {}

function PrintVec(vec)
    print(vec.x, " ", vec.y, " ", vec.z)
end

function PrintTable(table)
    for i = 1, #table do
        print(table[i])
    end
end

function Lerp(a, b, n)
    return b * n + a * (1. - n)
end

function Start()
    doorInitialPosition = node.transform.position
    Events.Subscribe("OpenSignal", OpenSignal)
end

function Update(dt)
    if shouldOpen then
        node.transform.position = Lerp(doorInitialPosition, doorDestination, timeSinceOpen / timeToOpen)
        timeSinceOpen = timeSinceOpen + dt
        if timeSinceOpen > timeToOpen then
            shouldOpen = false
            isOpen = true
        end
    end
end

function OpenSignal(buttonId)
    if isOpen then
        return
    end

    table.insert(codeState, buttonId)
    if #codeState == codeSize then
        for i = 1, codeSize do
            if codeState[i] ~= code[i] then
                codeState = {}
                print("incorrect code")
                return
            end
        end
        print("opening")
        shouldOpen = true
        timeSinceOpen = 0.
    end
end