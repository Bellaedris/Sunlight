Sunlight is a game editor powered by the Lumiere rendering engine. 
Lumiere is linked as a submodule in /ext/Lumiere.

## Build and install
Clone the project and initialize the submodules to fetch the rendering engine: 
```c
cd path/to/Sunlight
git submodule init
git submodule update
```

Configure and build the CMake project using your method of choice, I've been working with Clion but Visual studio should work fine, as well as command line or CMake GUI.

In case of a crash on startup with errors related to YAML nodes, it is possible that an artifact of serialized pipeline made its way into some commits, so ensure there is no .YAML file in the bin/config folder. 

## Features
The Sunlight editor is quite barebones for now.
### Editor
- The Rendering panel lists all render passes and allows to visualize their outputs and their order, and allows to edit their configuration.
- The Profiler panel profiles GPU frames and displays render times.
- The test panel allows to hot reload the shaders. If you want to edit them, they are located in bin/shaders.
- The Hierarchy panel allows the creation of Nodes. Nodes are kind of like Godot nodes, GameObjects that can hold components. A scene is a hierarchy of Nodes.
- The Inspector panel gives details about the currently selected node. All nodes have a Transform component, and you can add up to 3 different component types as of now:
    - Mesh Renderer -> select a valid mesh file (gltf, obj, fbx...) using the "select file" button, and it will be drawn in the scene.
    - Light -> Select a light type using the dropdown and edit its parameters
    - Script -> Sunlight uses Lua as its scripting language! Select a .lua file. The script can be refreshed with the refresh button. More details below.
  
### Scripting
A script must be a Lua file. The engine will try to find functions and to run them at each Update tick: 
  - Update() is called each frame
  - Start() is called before the first update

ONLY THESE functions will be called automatically. In these functions, you can interact with the engine components. For instance,
you can get the Transform of the Node owning the script with ```node.transform```. Any change on this object will be reflected on the C++ side.
Since i don't have documentation yet, check the Component classes (Lumiere/Components/*.cpp) to what type is registered to the lua 
context, and which methods are available for these types. 

As an example, this script tries to find a MeshRenderer on start, and displays its name if there is one, 
then moves the Node along a circular path each update:
```lua
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
    t.position = vec3(0, math.sin(elapsed), math.cos(elapsed))
    elapsed = elapsed + dt
end
```

## Interactions
You can move in the editor viewport with ZQSD, with A/E to move up/down. Hold alt to rotate the view.

The rendering settings can be changed at runtime and will be saved upon application closure. To restore the default parameters,
delete the YAML files in the bin/config folder.

The rendering packages two different view: Photorealistic and Cartoony. Right click to switch between the 2. They are still very very in progress!!