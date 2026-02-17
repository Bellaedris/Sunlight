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

## Features
The Sunlight is quite barebones for now. It includes mainly debugging tools. 
- The Rendering panel lists all render passes and allows to visualize their outputs and their order, and allows to edit their configuration.
- The Profiler panel profiles GPU frames and displays render times.
- The test panel allows to hot reload the shaders. If you want to edit them, they are located in bin/shaders.

## Interactions
You can move in the editor viewport with ZQSD, with A/E to move up/down. Hold alt to rotate the view.