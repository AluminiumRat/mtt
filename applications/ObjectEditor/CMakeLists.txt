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
    "include/AsyncTasks/LoadModelTask.h"
    "include/AsyncTasks/SaveModelTask.h"

    "include/Clipboard/CopyToClipboardOperation.h"
    "include/Clipboard/PasteFromClipboardOperation.h"

    "include/Objects/BoneRefBatch.h"
    "include/Objects/BoneRefObject.h"
    "include/Objects/GeometryGroup.h"
    "include/Objects/GeometryObject.h"
    "include/Objects/LODObject.h"
    "include/Objects/MaterialObject.h"
    "include/Objects/MaterialsGroup.h"
    "include/Objects/MeshObject.h"
    "include/Objects/MMDObjectFactory.h"
    "include/Objects/ObjectLoader.h"
    "include/Objects/ObjectSaver.h"
    "include/Objects/OEVisitor.h"
    "include/Objects/OEVisitorExtension.h"
    "include/Objects/RootObject.h"

    "include/PropertiesWidget/GeometryObjectWidget.h"
    "include/PropertiesWidget/LODObjectWidget.h"
    "include/PropertiesWidget/MaterialWidget.h"
    "include/PropertiesWidget/MeshWidget.h"
    "include/PropertiesWidget/PropertiesWidgetFactory.h"
    "include/PropertiesWidget/PropertiesWidget.h"
    "include/PropertiesWidget/VertexDataWidget.h"

    "include/Render/BonesObserver.h"
    "include/Render/LODRenderObserver.h"
    "include/Render/MaterialObserver.h"
    "include/Render/MeshRenderObserver.h"
    "include/Render/RenderObserverFactory.h"

    "include/EditMenu.h"
    "include/FbxDrawableImporter.h"
    "include/FileMenu.h"
    "include/MainWindow.h"
    "include/ObjectEditorCommonData.h"
    "include/ObjectEditorScene.h"
    )

set(SOURCES
    "src/AsyncTasks/AddModelFromFbxTask.cpp"
    "src/AsyncTasks/LoadModelTask.cpp"
    "src/AsyncTasks/SaveModelTask.cpp"

    "src/Clipboard/CopyToClipboardOperation.cpp"
    "src/Clipboard/PasteFromClipboardOperation.cpp"

    "src/Objects/BoneRefBatch.cpp"
    "src/Objects/BoneRefObject.cpp"
    "src/Objects/GeometryGroup.cpp"
    "src/Objects/GeometryObject.cpp"
    "src/Objects/LODObject.cpp"
    "src/Objects/MaterialObject.cpp"
    "src/Objects/MeshObject.cpp"
    "src/Objects/MMDObjectFactory.cpp"
    "src/Objects/ObjectLoader.cpp"
    "src/Objects/ObjectSaver.cpp"
    "src/Objects/OEVisitorExtension.cpp"
    "src/Objects/RootObject.cpp"

    "src/PropertiesWidget/GeometryObjectWidget.cpp"
    "src/PropertiesWidget/LODObjectWidget.cpp"
    "src/PropertiesWidget/MaterialWidget.cpp"
    "src/PropertiesWidget/MeshWidget.cpp"
    "src/PropertiesWidget/PropertiesWidgetFactory.cpp"
    "src/PropertiesWidget/PropertiesWidget.cpp"
    "src/PropertiesWidget/VertexDataWidget.cpp"

    "src/Render/BonesObserver.cpp"
    "src/Render/LODRenderObserver.cpp"
    "src/Render/MaterialObserver.cpp"
    "src/Render/MeshRenderObserver.cpp"
    "src/Render/RenderObserverFactory.cpp"

    "src/EditMenu.cpp"
    "src/FbxDrawableImporter.cpp"
    "src/FileMenu.cpp"
    "src/MainWindow.cpp"
    "src/ObjectEditor.cpp"
    "src/ObjectEditorCommonData.cpp"
    "src/ObjectEditorScene.cpp"
    )

set(UI_FILES
    "src/PropertiesWidget/GeometryObjectWidget.ui"
    "src/PropertiesWidget/LODObjectWidget.ui"
    "src/PropertiesWidget/MaterialWidget.ui"
    "src/PropertiesWidget/MeshWidget.ui"
    )

add_executable (ObjectEditor
                ${HEADERS}
                ${SOURCES}
                ${UI_FILES})

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
                      shaders
                      render
                      utilities)
