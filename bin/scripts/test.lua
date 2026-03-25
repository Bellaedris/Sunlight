local elapsed = 0

function Start()
    print("starting")
    local m = node:GetMeshComponent()
    if(m == nil) then
        print("No mesh found")
    else
        print(m.mesh:name())
    end

    local r = Ray(vec3(0, 0, 0), vec3(1, 0, 0), 10000)
    local result = Physics:Raycast(r)
    if(result ~= nil) then
        print(result.node:name())
    else
        print("no hit")
    end
end

function Update(dt)
    local t = node.transform
    -- t.position = vec3(t.position.x, t.position.y, math.cos(elapsed * 5) * 10)
    t.localPosition = vec3(0, math.sin(elapsed), math.cos(elapsed))
    --t:Translate(vec3(0, dt, 0))
    elapsed = elapsed + dt
end