cmake_minimum_required (VERSION 3.8)

project (ObjectEditor)

set(CMAKE_MODULE_PATH &{CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}../../../cmake)

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

find_path(VKMEMALLOC_PATH
          vk_mem_alloc.h
          HINTS "$ENV{VKMEMALLOC_PATH}"
          REQUIRED)

find_package(FbxSDK REQUIRED)

include_directories("include/"
                    "../../include/"
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
    "include/AsyncTasks/AddModelFromFbxTask.h"
    "include/AsyncTasks/LoadEnvironmentTask.h"
    "include/AsyncTasks/LoadModelTask.h"
    "include/AsyncTasks/ObjectBuilder.h"
    "include/AsyncTasks/ObjectLoader.h"
    "include/AsyncTasks/ObjectSaver.h"
    "include/AsyncTasks/SaveEnvironmentTask.h"
    "include/AsyncTasks/SaveModelTask.h"

    "include/Objects/BoneRefBatch.h"
    "include/Objects/BoneRefObject.h"
    "include/Objects/GeometryGroup.h"
    "include/Objects/GeometryObject.h"
    "include/Objects/LODObject.h"
    "include/Objects/MaterialObject.h"
    "include/Objects/MaterialsGroup.h"
    "include/Objects/MeshObject.h"
    "include/Objects/OEVisitor.h"
    "include/Objects/OEVisitorExtension.h"
    "include/Objects/RootObject.h"

    "include/Render/AbstractLightRenderObserver.h"
    "include/Render/AbstractObjectRenderObserver.h"
    "include/Render/AmbientLightRenderObserver.h"
    "include/Render/BackgroundObserver.h"
    "include/Render/BonesObserver.h"
    "include/Render/CubemapObserver.h"
    "include/Render/DirectLightRenderObserver.h"
    "include/Render/EditorRenderAction.h"
    "include/Render/EnvironmentModelRenderObserver.h"
    "include/Render/IconDrawableNode.h"
    "include/Render/LODRenderObserver.h"
    "include/Render/MaterialObserver.h"
    "include/Render/MeshRenderObserver.h"
    "include/Render/Object3DRenderObserver.h"
    "include/Render/SceneRenderObserver.h"
    "include/Render/SkeletonRenderObserver.h"

    "include/RenderWidget/EditorRenderWidget.h"

    "include/SceneTab/GeometryObjectWidget.h"
    "include/SceneTab/LODObjectWidget.h"
    "include/SceneTab/MaterialWidget.h"
    "include/SceneTab/MeshWidget.h"
    "include/SceneTab/PropertiesWidgetFactory.h"
    "include/SceneTab/SceneTab.h"
    "include/SceneTab/VertexDataWidget.h"

    "include/EditMenu.h"
    "include/EditorCommonData.h"
    "include/EditorScene.h"
    "include/FbxDrawableImporter.h"
    "include/FileMenu.h"
    "include/MainWindow.h"
    "include/ManipulatorMenu.h"
    )

set(SOURCES
    "src/AsyncTasks/AddModelFromFbxTask.cpp"
    "src/AsyncTasks/LoadEnvironmentTask.cpp"
    "src/AsyncTasks/LoadModelTask.cpp"
    "src/AsyncTasks/ObjectBuilder.cpp"
    "src/AsyncTasks/ObjectLoader.cpp"
    "src/AsyncTasks/ObjectSaver.cpp"
    "src/AsyncTasks/SaveEnvironmentTask.cpp"
    "src/AsyncTasks/SaveModelTask.cpp"

    "src/Objects/BoneRefBatch.cpp"
    "src/Objects/BoneRefObject.cpp"
    "src/Objects/GeometryGroup.cpp"
    "src/Objects/GeometryObject.cpp"
    "src/Objects/LODObject.cpp"
    "src/Objects/MaterialObject.cpp"
    "src/Objects/MeshObject.cpp"
    "src/Objects/OEVisitorExtension.cpp"
    "src/Objects/RootObject.cpp"

    "src/Render/AbstractLightRenderObserver.cpp"
    "src/Render/AbstractObjectRenderObserver.cpp"
    "src/Render/AmbientLightRenderObserver.cpp"
    "src/Render/BackgroundObserver.cpp"
    "src/Render/BonesObserver.cpp"
    "src/Render/CubemapObserver.cpp"
    "src/Render/DirectLightRenderObserver.cpp"
    "src/Render/EditorRenderAction.cpp"
    "src/Render/EnvironmentModelRenderObserver.cpp"
    "src/Render/IconDrawableNode.cpp"
    "src/Render/LODRenderObserver.cpp"
    "src/Render/MaterialObserver.cpp"
    "src/Render/MeshRenderObserver.cpp"
    "src/Render/Object3DRenderObserver.cpp"
    "src/Render/SceneRenderObserver.cpp"
    "src/Render/SkeletonRenderObserver.cpp"

    "src/RenderWidget/EditorRenderWidget.cpp"

    "src/SceneTab/GeometryObjectWidget.cpp"
    "src/SceneTab/LODObjectWidget.cpp"
    "src/SceneTab/MaterialWidget.cpp"
    "src/SceneTab/MeshWidget.cpp"
    "src/SceneTab/PropertiesWidgetFactory.cpp"
    "src/SceneTab/SceneTab.cpp"
    "src/SceneTab/VertexDataWidget.cpp"

    "src/EditMenu.cpp"
    "src/EditorCommonData.cpp"
    "src/EditorScene.cpp"
    "src/FbxDrawableImporter.cpp"
    "src/FileMenu.cpp"
    "src/MainWindow.cpp"
    "src/ManipulatorMenu.cpp"
    "src/ObjectEditor.cpp"
    )

set(UI_FILES
    "src/SceneTab/GeometryObjectWidget.ui"
    "src/SceneTab/LODObjectWidget.ui"
    "src/SceneTab/MaterialWidget.ui"
    "src/SceneTab/MeshWidget.ui"
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
                      application
                      clPipeline
                      editorLib
                      fbx
                      render
                      utilities)
