cmake_minimum_required (VERSION 3.8)

project(shaders)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Core REQUIRED)
set(CMAKE_AUTORCC ON)

add_definitions(${Qt5Core_DEFINITIONS})

add_library(shaders
            "shaders.qrc")

target_link_libraries(shaders
                      Qt5::Core)
