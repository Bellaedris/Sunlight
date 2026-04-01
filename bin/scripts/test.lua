local elapsed = 0

function Start()
    print("starting")
    Events.Subscribe("RaycastHit", OnRaycastHitBroadcast)
end

function Update(dt)
    -- t.position = vec3(t.position.x, t.position.y, math.cos(elapsed * 5) * 10)
    -- t.localPosition = vec3(0, math.sin(elapsed), math.cos(elapsed))
    --t:Translate(vec3(0, dt, 0))
    elapsed = elapsed + dt
end

function OnRaycastHitBroadcast()
    print("A raycast was hit. This object received the broadcast: ", node.name)
end

function RaycastHit(hitInfo)
    print(node.name, " was hit by a raycast with distance ", hitInfo.distance)
end