local elapsed = 0

function Start()
    print("starting")
    local m = node:GetMeshComponent()
    if(m == false) then
        print("No mesh found")
    else
        print(m.mesh:name())
    end
end

function Update(dt)
    local t = node.transform
    t.position = vec3(t.position.x, t.position.y, math.cos(elapsed * 5) * 10)
    -- t.position = vec3(0, math.sin(elapsed), math.cos(elapsed))
    --t:Translate(vec3(0, dt, 0))
    elapsed = elapsed + dt
end