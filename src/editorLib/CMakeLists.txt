cmake_minimum_required (VERSION 3.8)

project(editorLib)

set(CMAKE_MODULE_PATH &{CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_path(GLM_PATH
          glm/glm.hpp
          HINTS "$ENV{GLM_PATH}"
          REQUIRED)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Vulkan REQUIRED)

find_package(Shaderc REQUIRED)

find_package(FbxSDK REQUIRED)

include_directories("../../include"
                    ${GLM_PATH}
                    ${Vulkan_INCLUDE_DIRS}
                    ${Shaderc_INCLUDE_DIRS}
                    ${VKMEMALLOC_PATH}
                    ${FbxSDK_INCLUDE_DIRS}
                    )

add_definitions(${Qt5Core_DEFINITIONS}
                ${Qt5Widgets_DEFINITIONS}
                )

set(HEADERS
    "../../include/mtt/editorLib/Objects/AmbientLightObject.h"
    "../../include/mtt/editorLib/Objects/AnimationGroup.h"
    "../../include/mtt/editorLib/Objects/AnimationObject.h"
    "../../include/mtt/editorLib/Objects/AnimationTrack.h"
    "../../include/mtt/editorLib/Objects/BackgroundObject.h"
    "../../include/mtt/editorLib/Objects/CEVisitor.h"
    "../../include/mtt/editorLib/Objects/CEVisitorExtension.h"
    "../../include/mtt/editorLib/Objects/CubemapObject.h"
    "../../include/mtt/editorLib/Objects/DirectLightObject.h"
    "../../include/mtt/editorLib/Objects/DisplayedObject.h"
    "../../include/mtt/editorLib/Objects/EnvironmentGroup.h"
    "../../include/mtt/editorLib/Objects/EnvironmentModel.h"
    "../../include/mtt/editorLib/Objects/EnvironmentObject.h"
    "../../include/mtt/editorLib/Objects/LightObject.h"
    "../../include/mtt/editorLib/Objects/MovableObject.h"
    "../../include/mtt/editorLib/Objects/Object3D.h"
    "../../include/mtt/editorLib/Objects/RotatableObject.h"
    "../../include/mtt/editorLib/Objects/ScalableObject.h"
    "../../include/mtt/editorLib/Objects/SkeletonGroup.h"
    "../../include/mtt/editorLib/Objects/SkeletonObject.h"
    )

set(UI_FILES
    ""
    )

set(SOURCES
    "Objects/AmbientLightObject.cpp"
    "Objects/AnimationGroup.cpp"
    "Objects/AnimationObject.cpp"
    "Objects/AnimationTrack.cpp"
    "Objects/BackgroundObject.cpp"
    "Objects/CEVisitorExtension.cpp"
    "Objects/CubemapObject.cpp"
    "Objects/DirectLightObject.cpp"
    "Objects/DisplayedObject.cpp"
    "Objects/EnvironmentGroup.cpp"
    "Objects/EnvironmentModel.cpp"
    "Objects/EnvironmentObject.cpp"
    "Objects/LightObject.cpp"
    "Objects/MovableObject.cpp"
    "Objects/Object3D.cpp"
    "Objects/RotatableObject.cpp"
    "Objects/ScalableObject.cpp"
    "Objects/SkeletonGroup.cpp"
    "Objects/SkeletonObject.cpp"
    )

add_library(editorLib
            ${HEADERS}
            ${SOURCES}
            ${UI_FILES}
            )

target_link_libraries(editorLib
                      Qt5::Core
                      Qt5::Widgets
                      )
