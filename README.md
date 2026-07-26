# GLFW Tech Demo

## Description
This is a tech demo for an OpenGL GLFW project.
Basic template to start a bare bones OpenGL GLFW project using CMake.
The CMakePresets.json defines a debug build and a release build.
Tested in Visual Studio Code but it should work in the IDE of your choice.

## Setup in Visual Studio Code
Have the CMake extension installed.
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
