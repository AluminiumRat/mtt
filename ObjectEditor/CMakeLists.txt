cmake_minimum_required (VERSION 3.8)

project (ObjectEditor)

set(CMAKE_MODULE_PATH &{CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}../../cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Vulkan REQUIRED)

find_package(Shaderc REQUIRED)

find_path(GLM_PATH
          glm/glm.hpp
          HINTS "$ENV{GLM_PATH}"
          REQUIRED)

find_package(FbxSDK REQUIRED)

include_directories("include/"
                    "../include/"
                    ${GLM_PATH}
                    ${Vulkan_INCLUDE_DIRS}
                    ${Shaderc_INCLUDE_DIRS}
                    ${VKMEMALLOC_PATH}
                    ${FbxSDK_INCLUDE_DIRS}
                    )

add_definitions(${Qt5Core_DEFINITIONS}
                ${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(HEADERS
    "include/AsyncTasks/AddAnimationFromFbxTask.h"
    "include/AsyncTasks/AddModelFromFbxTask.h"
    "include/AsyncTasks/UploadCubetextureTask.h"
    "include/AsyncTasks/UploadTextureTask.h"

    "include/Manipulator/AxisManipulator.h"
    "include/Manipulator/EditorManipulatorController.h"
    "include/Manipulator/MoveManipulator.h"
    "include/Manipulator/ObjectManipulator.h"
    "include/Manipulator/PlaneManipulator.h"
    "include/Manipulator/RingManipulator.h"
    "include/Manipulator/RotationManipulator.h"
    "include/Manipulator/ScaleManipulator.h"
    "include/Manipulator/TriangleManipulator.h"

    "include/Objects/AmbientLightObject.h"
    "include/Objects/AnimationGroup.h"
    "include/Objects/AnimationObject.h"
    "include/Objects/AnimationTrack.h"
    "include/Objects/BackgroundObject.h"
    "include/Objects/BoneRefBatch.h"
    "include/Objects/BoneRefObject.h"
    "include/Objects/CubemapObject.h"
    "include/Objects/DirectLightObject.h"
    "include/Objects/DisplayedObject.h"
    "include/Objects/EnvironmentGroup.h"
    "include/Objects/EnvironmentObject.h"
    "include/Objects/GeometryGroup.h"
    "include/Objects/GeometryObject.h"
    "include/Objects/LightObject.h"
    "include/Objects/LODObject.h"
    "include/Objects/MaterialObject.h"
    "include/Objects/MaterialsGroup.h"
    "include/Objects/MeshObject.h"
    "include/Objects/MovableObject.h"
    "include/Objects/Object3D.h"
    "include/Objects/OEVisitor.h"
    "include/Objects/OEVisitorExtension.h"
    "include/Objects/RootObject.h"
    "include/Objects/RotatableObject.h"
    "include/Objects/ScalableObject.h"
    "include/Objects/SkeletonGroup.h"
    "include/Objects/SkeletonObject.h"

    "include/Render/AbstractLightRenderObserver.h"
    "include/Render/AbstractObjectRenderObserver.h"
    "include/Render/AmbientLightRenderObserver.h"
    "include/Render/BackgroundObserver.h"
    "include/Render/BonesObserver.h"
    "include/Render/CubemapObserver.h"
    "include/Render/DirectLightRenderObserver.h"
    "include/Render/EditorRenderAction.h"
    "include/Render/IconDrawableNode.h"
    "include/Render/LODRenderObserver.h"
    "include/Render/MaterialObserver.h"
    "include/Render/MeshRenderObserver.h"
    "include/Render/Object3DRenderObserver.h"
    "include/Render/SceneRenderObserver.h"
    "include/Render/SkeletonRenderObserver.h"

    "include/RenderWidget/EditorRenderWidget.h"

    "include/SceneTab/AmbientLightWidget.h"
    "include/SceneTab/AnimationObjectWidget.h"
    "include/SceneTab/AnimationTrackWidget.h"
    "include/SceneTab/BackgroundWidget.h"
    "include/SceneTab/CubemapWidget.h"
    "include/SceneTab/DirectLightWidget.h"
    "include/SceneTab/EditorObjectsItemModel.h"
    "include/SceneTab/GeometryObjectWidget.h"
    "include/SceneTab/LODObjectWidget.h"
    "include/SceneTab/MaterialWidget.h"
    "include/SceneTab/MeshWidget.h"
    "include/SceneTab/LightWidget.h"
    "include/SceneTab/MovableObjectWidget.h"
    "include/SceneTab/RotatableObjectWidget.h"
    "include/SceneTab/ScalableObjectWidget.h"
    "include/SceneTab/SceneTab.h"
    "include/SceneTab/SceneTreeView.h"
    "include/SceneTab/TexturePropertyWidget.h"
    "include/SceneTab/VertexDataWidget.h"
    "include/SceneTab/VisiblePropertyWidget.h"

    "include/AnimationPlayer.h"
    "include/EditMenu.h"
    "include/EditorApplication.h"
    "include/EditorCommonData.h"
    "include/EditorScene.h"
    "include/EditorUndoStack.h"
    "include/FbxAnimationImporter.h"
    "include/FbxDrawableImporter.h"
    "include/MainWindow.h"
    "include/ManipulatorMenu.h"
    )

set(SOURCES
    "src/AsyncTasks/AddAnimationFromFbxTask.cpp"
    "src/AsyncTasks/AddModelFromFbxTask.cpp"
    "src/AsyncTasks/UploadCubetextureTask.cpp"
    "src/AsyncTasks/UploadTextureTask.cpp"

    "src/Manipulator/AxisManipulator.cpp"
    "src/Manipulator/EditorManipulatorController.cpp"
    "src/Manipulator/MoveManipulator.cpp"
    "src/Manipulator/ObjectManipulator.cpp"
    "src/Manipulator/PlaneManipulator.cpp"
    "src/Manipulator/RingManipulator.cpp"
    "src/Manipulator/RotationManipulator.cpp"
    "src/Manipulator/ScaleManipulator.cpp"
    "src/Manipulator/TriangleManipulator.cpp"

    "src/Objects/AmbientLightObject.cpp"
    "src/Objects/AnimationGroup.cpp"
    "src/Objects/AnimationObject.cpp"
    "src/Objects/AnimationTrack.cpp"
    "src/Objects/BackgroundObject.cpp"
    "src/Objects/BoneRefBatch.cpp"
    "src/Objects/BoneRefObject.cpp"
    "src/Objects/CubemapObject.cpp"
    "src/Objects/DirectLightObject.cpp"
    "src/Objects/DisplayedObject.cpp"
    "src/Objects/EnvironmentGroup.cpp"
    "src/Objects/EnvironmentObject.cpp"
    "src/Objects/GeometryGroup.cpp"
    "src/Objects/GeometryObject.cpp"
    "src/Objects/LightObject.cpp"
    "src/Objects/LODObject.cpp"
    "src/Objects/MaterialObject.cpp"
    "src/Objects/MeshObject.cpp"
    "src/Objects/MovableObject.cpp"
    "src/Objects/Object3D.cpp"
    "src/Objects/OEVisitorExtension.cpp"
    "src/Objects/RootObject.cpp"
    "src/Objects/RotatableObject.cpp"
    "src/Objects/ScalableObject.cpp"
    "src/Objects/SkeletonGroup.cpp"
    "src/Objects/SkeletonObject.cpp"

    "src/Render/AbstractLightRenderObserver.cpp"
    "src/Render/AbstractObjectRenderObserver.cpp"
    "src/Render/AmbientLightRenderObserver.cpp"
    "src/Render/BackgroundObserver.cpp"
    "src/Render/BonesObserver.cpp"
    "src/Render/CubemapObserver.cpp"
    "src/Render/DirectLightRenderObserver.cpp"
    "src/Render/EditorRenderAction.cpp"
    "src/Render/IconDrawableNode.cpp"
    "src/Render/LODRenderObserver.cpp"
    "src/Render/MaterialObserver.cpp"
    "src/Render/MeshRenderObserver.cpp"
    "src/Render/Object3DRenderObserver.cpp"
    "src/Render/SceneRenderObserver.cpp"
    "src/Render/SkeletonRenderObserver.cpp"

    "src/RenderWidget/EditorRenderWidget.cpp"

    "src/SceneTab/AmbientLightWidget.cpp"
    "src/SceneTab/AnimationObjectWidget.cpp"
    "src/SceneTab/AnimationTrackWidget.cpp"
    "src/SceneTab/BackgroundWidget.cpp"
    "src/SceneTab/CubemapWidget.cpp"
    "src/SceneTab/DirectLightWidget.cpp"
    "src/SceneTab/EditorObjectsItemModel.cpp"
    "src/SceneTab/GeometryObjectWidget.cpp"
    "src/SceneTab/LODObjectWidget.cpp"
    "src/SceneTab/MaterialWidget.cpp"
    "src/SceneTab/MeshWidget.cpp"
    "src/SceneTab/LightWidget.cpp"
    "src/SceneTab/MovableObjectWidget.cpp"
    "src/SceneTab/RotatableObjectWidget.cpp"
    "src/SceneTab/ScalableObjectWidget.cpp"
    "src/SceneTab/SceneTab.cpp"
    "src/SceneTab/SceneTreeView.cpp"
    "src/SceneTab/VisiblePropertyWidget.cpp"
    "src/SceneTab/VertexDataWidget.cpp"

    "src/AnimationPlayer.cpp"
    "src/EditMenu.cpp"
    "src/EditorApplication.cpp"
    "src/EditorCommonData.cpp"
    "src/EditorScene.cpp"
    "src/EditorUndoStack.cpp"
    "src/FbxAnimationImporter.cpp"
    "src/FbxDrawableImporter.cpp"
    "src/MainWindow.cpp"
    "src/ManipulatorMenu.cpp"
    "src/ObjectEditor.cpp"
    )

set(UI_FILES
    "src/SceneTab/AmbientLightWidget.ui"
    "src/SceneTab/AnimationObjectWidget.ui"
    "src/SceneTab/AnimationTrackWidget.ui"
    "src/SceneTab/BackgroundWidget"
    "src/SceneTab/CubemapWidget.ui"
    "src/SceneTab/DirectLightWidget.ui"
    "src/SceneTab/GeometryObjectWidget.ui"
    "src/SceneTab/LODObjectWidget.ui"
    "src/SceneTab/MaterialWidget.ui"
    "src/SceneTab/MeshWidget.ui"
    "src/SceneTab/LightWidget.ui"
    "src/SceneTab/MovableObjectWidget.ui"
    "src/SceneTab/RotatableObjectWidget.ui"
    "src/SceneTab/ScalableObjectWidget.ui"
    "src/SceneTab/SceneTab.ui"
    "src/SceneTab/VisiblePropertyWidget.ui"
    "src/MainWindow.ui"
    )

set(QRC_FILES
    "resources/resources.qrc"
    )

add_executable (ObjectEditor
                ${HEADERS}
                ${SOURCES}
                ${UI_FILES}
                ${QRC_FILES})

target_link_libraries(ObjectEditor
                      ${Vulkan_LIBRARIES}
                      ${Shaderc_LIBRARIES}
                      ${FbxSDK_LIBRARIES}
                      Qt5::Core
                      Qt5::Widgets
                      Application
                      DrawImpl
                      Fbx
                      Render
                      Utilities)
