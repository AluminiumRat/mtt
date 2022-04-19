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
    "include/AsyncTasks/SaveEffectTask.h"
    "include/AsyncTasks/UploadParticleTexturesTask.h"

    "include/Objects/Fluid/BlockerObject.h"
    "include/Objects/Fluid/FluidMatrix.h"
    "include/Objects/Fluid/FluidModificator.h"
    "include/Objects/Fluid/FluidObject.h"
    "include/Objects/Fluid/GasSource.h"
    "include/Objects/Fluid/HeaterObject.h"
    "include/Objects/ActionAnimationTrack.h"
    "include/Objects/EmitParticlesAction.h"
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

    "include/PropertiesWidget/ActionAnimationTrackWidget.h"
    "include/PropertiesWidget/BlockerWidget.h"
    "include/PropertiesWidget/EmitParticlesActionWidget.h"
    "include/PropertiesWidget/EmitterWidget.h"
    "include/PropertiesWidget/FluidWidget.h"
    "include/PropertiesWidget/GasSourceWidget.h"
    "include/PropertiesWidget/GravityWidget.h"
    "include/PropertiesWidget/HeaterWidget.h"
    "include/PropertiesWidget/ParticleAnimationWidget.h"
    "include/PropertiesWidget/ParticleFieldWidget.h"
    "include/PropertiesWidget/PropertiesWidgetFactory.h"
    "include/PropertiesWidget/PropertiesWidget.h"
    "include/PropertiesWidget/SizeControlWidget.h"
    "include/PropertiesWidget/TypeMaskWidget.h"
    "include/PropertiesWidget/VisibilityControlWidget.h"

    "include/Render/BlockerRenderObserver.h"
    "include/Render/DrawParticlesAction.h"
    "include/Render/EmitterRenderObserver.h"
    "include/Render/FieldRenderObserver.h"
    "include/Render/FluidModificatorObserver.h"
    "include/Render/ObserverWithIcon.h"
    "include/Render/ParticlesDrawable.h"
    "include/Render/RenderObserverFactory.h"

    "include/EditMenu.h"
    "include/FileMenu.h"
    "include/MainWindow.h"
    "include/ParticlesEditorCommonData.h"
    "include/ParticlesEditorScene.h"
    )

set(SOURCES
    "src/AsyncTasks/ImportAnimationTask.cpp"
    "src/AsyncTasks/LoadEffectTask.cpp"
    "src/AsyncTasks/UploadParticleTexturesTask.cpp"
    "src/AsyncTasks/SaveEffectTask.cpp"

    "src/Objects/Fluid/BlockerObject.cpp"
    "src/Objects/Fluid/FluidModificator.cpp"
    "src/Objects/Fluid/FluidObject.cpp"
    "src/Objects/Fluid/GasSource.cpp"
    "src/Objects/Fluid/HeaterObject.cpp"
    "src/Objects/ActionAnimationTrack.cpp"
    "src/Objects/EmitParticlesAction.cpp"
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

    "src/PropertiesWidget/ActionAnimationTrackWidget.cpp"
    "src/PropertiesWidget/BlockerWidget.cpp"
    "src/PropertiesWidget/EmitParticlesActionWidget.cpp"
    "src/PropertiesWidget/EmitterWidget.cpp"
    "src/PropertiesWidget/FluidWidget.cpp"
    "src/PropertiesWidget/GasSourceWidget.cpp"
    "src/PropertiesWidget/GravityWidget.cpp"
    "src/PropertiesWidget/HeaterWidget.cpp"
    "src/PropertiesWidget/ParticleAnimationWidget.cpp"
    "src/PropertiesWidget/ParticleFieldWidget.cpp"
    "src/PropertiesWidget/PropertiesWidgetFactory.cpp"
    "src/PropertiesWidget/PropertiesWidget.cpp"
    "src/PropertiesWidget/SizeControlWidget.cpp"
    "src/PropertiesWidget/TypeMaskWidget.cpp"
    "src/PropertiesWidget/VisibilityControlWidget.cpp"

    "src/Render/BlockerRenderObserver.cpp"
    "src/Render/DrawParticlesAction.cpp"
    "src/Render/EmitterRenderObserver.cpp"
    "src/Render/FieldRenderObserver.cpp"
    "src/Render/FluidModificatorObserver.cpp"
    "src/Render/ObserverWithIcon.cpp"
    "src/Render/ParticlesDrawable.cpp"
    "src/Render/RenderObserverFactory.cpp"

    "src/EditMenu.cpp"
    "src/FileMenu.cpp"
    "src/MainWindow.cpp"
    "src/ParticlesEditor.cpp"
    "src/ParticlesEditorCommonData.cpp"
    "src/ParticlesEditorScene.cpp"
    )

set(UI_FILES
    "src/PropertiesWidget/ActionAnimationTrackWidget.ui"
    "src/PropertiesWidget/BlockerWidget.ui"
    "src/PropertiesWidget/EmitParticlesActionWidget.ui"
    "src/PropertiesWidget/EmitterWidget.ui"
    "src/PropertiesWidget/FluidWidget.ui"
    "src/PropertiesWidget/GasSourceWidget.ui"
    "src/PropertiesWidget/GravityWidget.ui"
    "src/PropertiesWidget/HeaterWidget.ui"
    "src/PropertiesWidget/ParticleAnimationWidget.ui"
    "src/PropertiesWidget/ParticleFieldWidget.ui"
    "src/PropertiesWidget/SizeControlWidget.ui"
    "src/PropertiesWidget/TypeMaskWidget.ui"
    "src/PropertiesWidget/VisibilityControlWidget.ui"
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
