# GLFW Tech Demo

## Description
This is a personal practice project for creating a 2D game engine in C++.
The goal is to learn more about working with different frameworks and using the ECS architecture.
This is not a project with an end result in mind. It is more of a testbed to create small tech demos.

## Architecture
The architecture is ECS with a focus on hexagonal design.
On the root level the vendor directory contains all third party libraries and the src directory contains custom code.
<br/>
The compressor directory is not really part of the engine. It has isolated logic and is used to compress asset files. Read more about it in [Asset compression](#asset-compression).
<br/>
<br/>
The hierarchy of the directory structure is shown below. The order determines accessibility. For example, engine/builder is allowed to access engine/core, but not vice versa.
<br/>
- compressor
- component
- engine
    - config
    - util
    - core
    - builder
    - adapter
        - asset
        - ....
        - systems
        - scene
<br/>
The component directory contains all ECS components and is the backbone of the architecture. The implementation inside the core directory forms the basis for hexagonal design.
Interfaces are used to separate the core logic from the inner workings of the engine. Adapters are used to handle specific logic, such as rendering or sound.

## Setup in Visual Studio Code
This project is created in Visual Studio Code but using CMake it should work in the IDE of your choice.
Have the CMake extension installed.
The CMakePresets.json defines a debug build and a release build.
To change between debug and release configuration, go to the CMake extension tool (most likely on the left side of the window).
Navigate to "Configure". Here you should be able to select either "debug" or "release".

## Quick start
Rename the sample_data.pak file to data.pak.
Build a debug or release build, then move the data.pak to the directory where the progam exe file is.

## Asset compression
Individual asset files are not read directly from the path.
Instead, the engine expects to retrieve them from a compressed file called data.pak.
The idea is that, in the end product, you want to somewhat protect your individual assets from the end user.
Of course, a tech savvy person could still get them. But using a compressed file is a good first line of defense.<br/><br/>
There is a small separate code base for a compressor tool in /src/compressor/.
This will generate a compressor executable. This executable will compress all files in the asset directory in a file called data.pak.
In order to use it, build the project, mode compressor.exe to the base directory (which contains the assets directory).
Then run the post_build.bat. This is a small script that will generate a data.pak file.
This data.pak file needs to be put in the same directory as the tech demo executable.
