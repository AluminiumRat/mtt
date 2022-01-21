cmake_minimum_required (VERSION 3.8)

project(fbx)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_path(GLM_PATH
          glm/glm.hpp
          HINTS "$ENV{GLM_PATH}"
          REQUIRED)

find_package(Qt5Core REQUIRED)

set(CMAKE_MODULE_PATH &{CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
find_package(FbxSDK REQUIRED)

include_directories("../../include"
                    ${FbxSDK_INCLUDE_DIRS}
                    ${GLM_PATH}
                    )

add_definitions(${Qt5Core_DEFINITIONS})

set(HEADERS
    "../../include/mtt/fbx/BaseFbxImporter.h"
    "../../include/mtt/fbx/FbxLib.h"
    "../../include/mtt/fbx/FbxRAII.h"
    )

set(SOURCES
    "BaseFbxImporter.cpp"
    "FbxLib.cpp"
    )

add_library(fbx
            ${HEADERS}
            ${SOURCES})

target_link_libraries(fbx
                      Qt5::Core
                      )
