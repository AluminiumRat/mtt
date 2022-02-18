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

    "../../include/mtt/editorLib/SceneTab/AmbientLightWidget.h"
    "../../include/mtt/editorLib/SceneTab/AnimationObjectWidget.h"
    "../../include/mtt/editorLib/SceneTab/AnimationTrackWidget.h"
    "../../include/mtt/editorLib/SceneTab/BackgroundWidget.h"
    "../../include/mtt/editorLib/SceneTab/CubemapWidget.h"
    "../../include/mtt/editorLib/SceneTab/DirectLightWidget.h"
    "../../include/mtt/editorLib/SceneTab/EditorObjectsItemModel.h"
    "../../include/mtt/editorLib/SceneTab/EnvironmentModelWidget.h"
    "../../include/mtt/editorLib/SceneTab/LightWidget.h"
    "../../include/mtt/editorLib/SceneTab/MovableObjectWidget.h"
    "../../include/mtt/editorLib/SceneTab/PropertiesWidgetFactory.h"
    "../../include/mtt/editorLib/SceneTab/RotatableObjectWidget.h"
    "../../include/mtt/editorLib/SceneTab/ScalableObjectWidget.h"
    "../../include/mtt/editorLib/SceneTab/SceneTab.h"
    "../../include/mtt/editorLib/SceneTab/SceneTreeView.h"
    "../../include/mtt/editorLib/SceneTab/VisiblePropertyWidget.h"

    "../../include/mtt/editorLib/AnimationPlayer.h"
    "../../include/mtt/editorLib/EditorUndoStack.h"
    )

set(UI_FILES
    "SceneTab/AmbientLightWidget.ui"
    "SceneTab/AnimationObjectWidget.ui"
    "SceneTab/AnimationTrackWidget.ui"
    "SceneTab/BackgroundWidget.ui"
    "SceneTab/CubemapWidget.ui"
    "SceneTab/DirectLightWidget.ui"
    "SceneTab/EnvironmentModelWidget.ui"
    "SceneTab/LightWidget.ui"
    "SceneTab/MovableObjectWidget.ui"
    "SceneTab/RotatableObjectWidget.ui"
    "SceneTab/ScalableObjectWidget.ui"
    "SceneTab/SceneTab.ui"
    "SceneTab/VisiblePropertyWidget.ui"
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

    "SceneTab/AmbientLightWidget.cpp"
    "SceneTab/AnimationObjectWidget.cpp"
    "SceneTab/AnimationTrackWidget.cpp"
    "SceneTab/BackgroundWidget.cpp"
    "SceneTab/CubemapWidget.cpp"
    "SceneTab/DirectLightWidget.cpp"
    "SceneTab/EditorObjectsItemModel.cpp"
    "SceneTab/EnvironmentModelWidget.cpp"
    "SceneTab/LightWidget.cpp"
    "SceneTab/MovableObjectWidget.cpp"
    "SceneTab/PropertiesWidgetFactory.cpp"
    "SceneTab/RotatableObjectWidget.cpp"
    "SceneTab/ScalableObjectWidget.cpp"
    "SceneTab/SceneTab.cpp"
    "SceneTab/SceneTreeView.cpp"
    "SceneTab/VisiblePropertyWidget.cpp"

    "AnimationPlayer.cpp"
    "EditorUndoStack.cpp"
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
