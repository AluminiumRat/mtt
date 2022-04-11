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

find_path(VKMEMALLOC_PATH
          vk_mem_alloc.h
          HINTS "$ENV{VKMEMALLOC_PATH}"
          REQUIRED)

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
    "../../include/mtt/editorLib/AsyncTasks/AddAnimationFromFbxTask.h"
    "../../include/mtt/editorLib/AsyncTasks/LoadEnvironmentModelTask.h"
    "../../include/mtt/editorLib/AsyncTasks/LoadEnvironmentTask.h"
    "../../include/mtt/editorLib/AsyncTasks/SaveEnvironmentTask.h"
    "../../include/mtt/editorLib/AsyncTasks/UploadCubetextureTask.h"
    "../../include/mtt/editorLib/AsyncTasks/UploadTextureTask.h"

    "../../include/mtt/editorLib/Manipulator/AxisManipulator.h"
    "../../include/mtt/editorLib/Manipulator/EditorManipulatorController.h"
    "../../include/mtt/editorLib/Manipulator/MoveManipulator.h"
    "../../include/mtt/editorLib/Manipulator/ObjectManipulator.h"
    "../../include/mtt/editorLib/Manipulator/PlaneManipulator.h"
    "../../include/mtt/editorLib/Manipulator/RingManipulator.h"
    "../../include/mtt/editorLib/Manipulator/RotationManipulator.h"
    "../../include/mtt/editorLib/Manipulator/ScaleManipulator.h"
    "../../include/mtt/editorLib/Manipulator/TriangleManipulator.h"

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
    "../../include/mtt/editorLib/Objects/EnvironmentObjectFactory.h"
    "../../include/mtt/editorLib/Objects/EnvironmentRootObject.h"
    "../../include/mtt/editorLib/Objects/LightObject.h"
    "../../include/mtt/editorLib/Objects/MovableObject.h"
    "../../include/mtt/editorLib/Objects/Object3D.h"
    "../../include/mtt/editorLib/Objects/ObjectFactory.h"
    "../../include/mtt/editorLib/Objects/ObjectLoader.h"
    "../../include/mtt/editorLib/Objects/ObjectSaver.h"
    "../../include/mtt/editorLib/Objects/RotatableObject.h"
    "../../include/mtt/editorLib/Objects/ScalableObject.h"
    "../../include/mtt/editorLib/Objects/SkeletonGroup.h"
    "../../include/mtt/editorLib/Objects/SkeletonObject.h"

    "../../include/mtt/editorLib/Render/AbstractLightRenderObserver.h"
    "../../include/mtt/editorLib/Render/AbstractObjectRenderObserver.h"
    "../../include/mtt/editorLib/Render/AmbientLightRenderObserver.h"
    "../../include/mtt/editorLib/Render/BackgroundObserver.h"
    "../../include/mtt/editorLib/Render/CubemapObserver.h"
    "../../include/mtt/editorLib/Render/CrossRenderObserver.h"
    "../../include/mtt/editorLib/Render/DirectLightRenderObserver.h"
    "../../include/mtt/editorLib/Render/EditorRenderAction.h"
    "../../include/mtt/editorLib/Render/EditorRenderWidget.h"
    "../../include/mtt/editorLib/Render/EnvironmentModelRenderObserver.h"
    "../../include/mtt/editorLib/Render/HullDrawableNode.h"
    "../../include/mtt/editorLib/Render/IconDrawableNode.h"
    "../../include/mtt/editorLib/Render/Object3DRenderObserver.h"
    "../../include/mtt/editorLib/Render/RenderObserverFactory.h"
    "../../include/mtt/editorLib/Render/SceneRenderObserver.h"

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
    "../../include/mtt/editorLib/EditorApplication.h"
    "../../include/mtt/editorLib/EditorScene.h"
    "../../include/mtt/editorLib/EditorUndoStack.h"
    "../../include/mtt/editorLib/FbxAnimationImporter.h"
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
    "AsyncTasks/AddAnimationFromFbxTask.cpp"
    "AsyncTasks/LoadEnvironmentModelTask.cpp"
    "AsyncTasks/LoadEnvironmentTask.cpp"
    "AsyncTasks/SaveEnvironmentTask.cpp"
    "AsyncTasks/UploadCubetextureTask.cpp"
    "AsyncTasks/UploadTextureTask.cpp"

    "Manipulator/AxisManipulator.cpp"
    "Manipulator/EditorManipulatorController.cpp"
    "Manipulator/MoveManipulator.cpp"
    "Manipulator/ObjectManipulator.cpp"
    "Manipulator/PlaneManipulator.cpp"
    "Manipulator/RingManipulator.cpp"
    "Manipulator/RotationManipulator.cpp"
    "Manipulator/ScaleManipulator.cpp"
    "Manipulator/TriangleManipulator.cpp"

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
    "Objects/EnvironmentObjectFactory.cpp"
    "Objects/EnvironmentRootObject.cpp"
    "Objects/LightObject.cpp"
    "Objects/MovableObject.cpp"
    "Objects/Object3D.cpp"
    "Objects/ObjectFactory.cpp"
    "Objects/ObjectLoader.cpp"
    "Objects/ObjectSaver.cpp"
    "Objects/RotatableObject.cpp"
    "Objects/ScalableObject.cpp"
    "Objects/SkeletonGroup.cpp"
    "Objects/SkeletonObject.cpp"

    "Render/AbstractLightRenderObserver.cpp"
    "Render/AbstractObjectRenderObserver.cpp"
    "Render/AmbientLightRenderObserver.cpp"
    "Render/BackgroundObserver.cpp"
    "Render/CrossRenderObserver.cpp"
    "Render/CubemapObserver.cpp"
    "Render/DirectLightRenderObserver.cpp"
    "Render/EditorRenderAction.cpp"
    "Render/EditorRenderWidget.cpp"
    "Render/EnvironmentModelRenderObserver.cpp"
    "Render/HullDrawableNode.cpp"
    "Render/IconDrawableNode.cpp"
    "Render/Object3DRenderObserver.cpp"
    "Render/RenderObserverFactory.cpp"
    "Render/SceneRenderObserver.cpp"

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
    "EditorApplication.cpp"
    "EditorScene.cpp"
    "EditorUndoStack.cpp"
    "FbxAnimationImporter.cpp"
    )

set(QRC_FILES
    "resources/editorLibResources.qrc"
    )

add_library(editorLib
            ${HEADERS}
            ${SOURCES}
            ${UI_FILES}
            ${QRC_FILES})

target_link_libraries(editorLib
                      Qt5::Core
                      Qt5::Widgets
                      )
