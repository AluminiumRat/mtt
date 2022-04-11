cmake_minimum_required (VERSION 3.8)

project (ParticlesEditor)

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
    "include/AsyncTasks/ImportAnimationTask.h"
    "include/AsyncTasks/LoadEffectTask.h"
    "include/AsyncTasks/LoadEnvironmentTask.h"
    "include/AsyncTasks/SaveEffectTask.h"
    "include/AsyncTasks/UploadParticleTexturesTask.h"

    "include/Objects/Fluid/BlockerObject.h"
    "include/Objects/Fluid/FluidMatrix.h"
    "include/Objects/Fluid/FluidModificator.h"
    "include/Objects/Fluid/FluidObject.h"
    "include/Objects/Fluid/GasSource.h"
    "include/Objects/Fluid/HeaterObject.h"
    "include/Objects/EmitterObject.h"
    "include/Objects/FrameObject.h"
    "include/Objects/GravityModificator.h"
    "include/Objects/HierarhicalObject.h"
    "include/Objects/ModificatorGroup.h"
    "include/Objects/ModificatorObject.h"
    "include/Objects/ObjectLoader.h"
    "include/Objects/ObjectSaver.h"
    "include/Objects/ParticleAnimation.h"
    "include/Objects/ParticleField.h"
    "include/Objects/ParticleTextureDescription.h"
    "include/Objects/PEEObjectFactory.h"
    "include/Objects/PEVisitor.h"
    "include/Objects/PEVisitorExtension.h"
    "include/Objects/RootObject.h"
    "include/Objects/SizeControlObject.h"
    "include/Objects/VisibilityControlObject.h"

    "include/Render/BlockerRenderObserver.h"
    "include/Render/DrawParticlesAction.h"
    "include/Render/EmitterRenderObserver.h"
    "include/Render/FieldRenderObserver.h"
    "include/Render/FluidModificatorObserver.h"
    "include/Render/ObserverWithIcon.h"
    "include/Render/ParticlesDrawable.h"
    "include/Render/RenderObserverFactory.h"

    "include/SceneTab/BlockerWidget.h"
    "include/SceneTab/EmitterWidget.h"
    "include/SceneTab/FluidWidget.h"
    "include/SceneTab/GasSourceWidget.h"
    "include/SceneTab/GravityWidget.h"
    "include/SceneTab/HeaterWidget.h"
    "include/SceneTab/ParticleAnimationWidget.h"
    "include/SceneTab/ParticleFieldWidget.h"
    "include/SceneTab/PropertiesWidgetFactory.h"
    "include/SceneTab/SceneTab.h"
    "include/SceneTab/SizeControlWidget.h"
    "include/SceneTab/TypeMaskWidget.h"
    "include/SceneTab/VisibilityControlWidget.h"

    "include/EditMenu.h"
    "include/EditorCommonData.h"
    "include/FileMenu.h"
    "include/MainWindow.h"
    "include/ManipulatorMenu.h"
    "include/ParticlesEditorScene.h"
    )

set(SOURCES
    "src/AsyncTasks/ImportAnimationTask.cpp"
    "src/AsyncTasks/LoadEffectTask.cpp"
    "src/AsyncTasks/LoadEnvironmentTask.cpp"
    "src/AsyncTasks/UploadParticleTexturesTask.cpp"
    "src/AsyncTasks/SaveEffectTask.cpp"

    "src/Objects/Fluid/BlockerObject.cpp"
    "src/Objects/Fluid/FluidModificator.cpp"
    "src/Objects/Fluid/FluidObject.cpp"
    "src/Objects/Fluid/GasSource.cpp"
    "src/Objects/Fluid/HeaterObject.cpp"
    "src/Objects/EmitterObject.cpp"
    "src/Objects/FrameObject.cpp"
    "src/Objects/GravityModificator.cpp"
    "src/Objects/HierarhicalObject.cpp"
    "src/Objects/ModificatorGroup.cpp"
    "src/Objects/ModificatorObject.cpp"
    "src/Objects/ObjectLoader.cpp"
    "src/Objects/ObjectSaver.cpp"
    "src/Objects/ParticleAnimation.cpp"
    "src/Objects/ParticleField.cpp"
    "src/Objects/PEEObjectFactory.cpp"
    "src/Objects/PEVisitorExtension.cpp"
    "src/Objects/RootObject.cpp"
    "src/Objects/SizeControlObject.cpp"
    "src/Objects/VisibilityControlObject.cpp"

    "src/Render/BlockerRenderObserver.cpp"
    "src/Render/DrawParticlesAction.cpp"
    "src/Render/EmitterRenderObserver.cpp"
    "src/Render/FieldRenderObserver.cpp"
    "src/Render/FluidModificatorObserver.cpp"
    "src/Render/ObserverWithIcon.cpp"
    "src/Render/ParticlesDrawable.cpp"
    "src/Render/RenderObserverFactory.cpp"

    "src/SceneTab/BlockerWidget.cpp"
    "src/SceneTab/EmitterWidget.cpp"
    "src/SceneTab/FluidWidget.cpp"
    "src/SceneTab/GasSourceWidget.cpp"
    "src/SceneTab/GravityWidget.cpp"
    "src/SceneTab/HeaterWidget.cpp"
    "src/SceneTab/ParticleAnimationWidget.cpp"
    "src/SceneTab/ParticleFieldWidget.cpp"
    "src/SceneTab/PropertiesWidgetFactory.cpp"
    "src/SceneTab/SceneTab.cpp"
    "src/SceneTab/SizeControlWidget.cpp"
    "src/SceneTab/TypeMaskWidget.cpp"
    "src/SceneTab/VisibilityControlWidget.cpp"

    "src/EditMenu.cpp"
    "src/EditorCommonData.cpp"
    "src/FileMenu.cpp"
    "src/MainWindow.cpp"
    "src/ManipulatorMenu.cpp"
    "src/ParticlesEditor.cpp"
    "src/ParticlesEditorScene.cpp"
    )

set(UI_FILES
    "src/SceneTab/EmitterWidget.ui"
    "src/SceneTab/FluidWidget.ui"
    "src/SceneTab/GasSourceWidget.ui"
    "src/SceneTab/GravityWidget.ui"
    "src/SceneTab/HeaterWidget.ui"
    "src/SceneTab/ParticleAnimationWidget.ui"
    "src/SceneTab/ParticleFieldWidget.ui"
    "src/SceneTab/SizeControlWidget.ui"
    "src/SceneTab/TypeMaskWidget.ui"
    "src/SceneTab/VisibilityControlWidget.ui"
    "src/MainWindow.ui"
    )

set(QRC_FILES
    "resources/resources.qrc"
    )

add_executable (ParticlesEditor
                ${HEADERS}
                ${SOURCES}
                ${UI_FILES}
                ${QRC_FILES})

target_link_libraries(ParticlesEditor
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
