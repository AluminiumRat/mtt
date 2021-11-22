# mtt
Toolset for creating graphical applications.
This is simple 

#How to build
To date, i have only tested building on Windows and Visual Studio 2022.
You well need the following components:
* С++20 compiler (I am using Visual Studio 2019 and Visual Studio 2022)
* CMake
* Vulkan SDK(https://www.lunarg.com/vulkan-sdk/)
* Vulkan Memory Allocator(https://gpuopen.com/vulkan-memory-allocator/)
* GLM library(https://github.com/g-truc/glm)
* Qt5
* Fbx SDK

Most likely, you will need to specify the location of some libraries for CMake. Add Qt cmake path, DbxSDK main path, FbxSDK lib path, glm and vulkan memory allocator paths to CMAKE_PREFIX_PATH environment variable(or build directive). For example "C:\Qt\5.15.2\msvc2019_64\lib\cmake;C:\Program Files\Autodesk\FBX\FBX SDK\2020.0.1;C:\Program Files\Autodesk\FBX\FBX SDK\2020.0.1\lib\vs2017\x64;C:\glm;C:\VulkanMemoryAllocator\include"

You can use cmake-gui or build from command line:
  mkdir build
  cd build
  cmake ..
  cmake --build .

The result will be written to the bin and lib subdirectories of the build folder.
