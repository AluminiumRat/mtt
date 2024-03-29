cmake_minimum_required (VERSION 3.8)

project(application)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_path(GLM_PATH
          glm/glm.hpp
          HINTS "$ENV{GLM_PATH}"
          REQUIRED)

find_path(VKMEMALLOC_PATH
          vk_mem_alloc.h
          HINTS "$ENV{VKMEMALLOC_PATH}"
          REQUIRED)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Vulkan REQUIRED)

set(CMAKE_MODULE_PATH &{CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
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
    "../../include/mtt/application/AsyncTasks/AbstractAsyncTask.h"
    "../../include/mtt/application/AsyncTasks/AsyncTaskQueue.h"
    "../../include/mtt/application/AsyncTasks/SaveToFileTask.h"

    "../../include/mtt/application/Clipboard/CopyToClipboardOperation.h"
    "../../include/mtt/application/Clipboard/PasteFromClipboardOperation.h"

    "../../include/mtt/application/DrawModel/AbstractDrawModelLoader.h"
    "../../include/mtt/application/DrawModel/DrawModelAnimation.h"
    "../../include/mtt/application/DrawModel/DrawModelLoaderRegistry.h"
    "../../include/mtt/application/DrawModel/FbxModelLoader.h"
    "../../include/mtt/application/DrawModel/DrawModelAnimationTrack.h"
    "../../include/mtt/application/DrawModel/MasterDrawModel.h"
    "../../include/mtt/application/DrawModel/MeshControlNode.h"
    "../../include/mtt/application/DrawModel/MMDModelLoader.h"
    "../../include/mtt/application/DrawModel/SkinControlNode.h"
    "../../include/mtt/application/DrawModel/SlaveDrawModel.h"
    "../../include/mtt/application/DrawModel/TransformTable.h"

    "../../include/mtt/application/EditCommands/AbstractEditCommand.h"
    "../../include/mtt/application/EditCommands/AddObjectCommand.h"
    "../../include/mtt/application/EditCommands/CompositeCommand.h"
    "../../include/mtt/application/EditCommands/MoveObjectCommand.h"
    "../../include/mtt/application/EditCommands/RemoveObjectCommand.h"
    "../../include/mtt/application/EditCommands/SetPropertyCommand.h"
    "../../include/mtt/application/EditCommands/SetReferenceCommand.h"
    "../../include/mtt/application/EditCommands/UndoStack.h"

    "../../include/mtt/application/Manipulator/AbstractManipulator.h"
    "../../include/mtt/application/Manipulator/AxisMove3DManipulator.h"
    "../../include/mtt/application/Manipulator/Composite3DManipulator.h"
    "../../include/mtt/application/Manipulator/Manipulator3D.h"
    "../../include/mtt/application/Manipulator/ManipulatorController.h"
    "../../include/mtt/application/Manipulator/PlaneMove3DManipulator.h"
    "../../include/mtt/application/Manipulator/RingRotation3DManipulator.h"
    "../../include/mtt/application/Manipulator/Simple3DManipulator.h"
    "../../include/mtt/application/Manipulator/UniformScale3DManipulator.h"

    "../../include/mtt/application/ResourceManager/CubeTextureDescription.h"
    "../../include/mtt/application/ResourceManager/CubeTextureLibrary.h"
    "../../include/mtt/application/ResourceManager/DrawModelDescription.h"
    "../../include/mtt/application/ResourceManager/DrawModelLibrary.h"
    "../../include/mtt/application/ResourceManager/ResourceLibrary.h"
    "../../include/mtt/application/ResourceManager/Texture2DDescription.h"
    "../../include/mtt/application/ResourceManager/Texture2DLibrary.h"

    "../../include/mtt/application/Scene/Object.h"
    "../../include/mtt/application/Scene/ObjectFactory.h"
    "../../include/mtt/application/Scene/ObjectGroup.h"
    "../../include/mtt/application/Scene/ObjectItemListModel.h"
    "../../include/mtt/application/Scene/ObjectItemTreeModel.h"
    "../../include/mtt/application/Scene/ObjectLink.h"
    "../../include/mtt/application/Scene/ObjectLoader.h"
    "../../include/mtt/application/Scene/ObjectSaver.h"
    "../../include/mtt/application/Scene/ObjectVisitor.h"
    "../../include/mtt/application/Scene/Scene.h"
    "../../include/mtt/application/Scene/SpecialGroup.h"

    "../../include/mtt/application/Widgets/Actions/CopyAction.h"
    "../../include/mtt/application/Widgets/Actions/DeleteAction.h"
    "../../include/mtt/application/Widgets/Actions/PasteAction.h"
    "../../include/mtt/application/Widgets/Actions/RedoAction.h"
    "../../include/mtt/application/Widgets/Actions/UndoAction.h"
    "../../include/mtt/application/Widgets/AsyncTaskDialog/AsyncTaskDialog.h"
    "../../include/mtt/application/Widgets/AsyncTaskDialog/TaskProgressWidget.h"

    "../../include/mtt/application/Widgets/PropertiesWidgets/Bitfield32PropertyConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/Bitfield32Widget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/BoolPropertyWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/ColorWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/EnumComboConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/FilenamePropertyWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/FloatPropertyLinearSlider.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/FloatPropertyProgressiveSlider.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/FloatPropertySpin.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/FloatRangeSpinConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/IntPropertySpin.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/NamePropertyWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/ReferenceWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/RotationPropertyWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/StringPropertyWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/TimeRangeSpinConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/TimeSpinConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/Vec3SpinConnection.h"
    "../../include/mtt/application/Widgets/PropertiesWidgets/Vec3Widget.h"

    "../../include/mtt/application/Widgets/RenderWidget/ClickSelectHandler.h"
    "../../include/mtt/application/Widgets/RenderWidget/OrbitalCameraController.h"
    "../../include/mtt/application/Widgets/RenderWidget/RenderWidget.h"

    "../../include/mtt/application/Widgets/ObjectsTreeView.h"
    "../../include/mtt/application/Widgets/SelectObjectsDialog.h"
    "../../include/mtt/application/Widgets/StatisticWidget.h"

    "../../include/mtt/application/WorkCycle/RenderImageAction.h"
    "../../include/mtt/application/WorkCycle/RenderSceneAction.h"
    "../../include/mtt/application/WorkCycle/WorkCycle.h"

    "../../include/mtt/application/Application.h"
    "../../include/mtt/application/ApplicationShaderLoader.h"
    "../../include/mtt/application/ApplicationStatistic.h"
    "../../include/mtt/application/CommonEditData.h"
    "../../include/mtt/application/DataStream.h"
    "../../include/mtt/application/TimeT.h"
    )

set(UI_FILES
    "Widgets/AsyncTaskDialog/TaskProgressWidget.ui"
    "Widgets/PropertiesWidgets/Bitfield32Widget.ui"
    "Widgets/PropertiesWidgets/ColorPropertyWidget.ui"
    "Widgets/PropertiesWidgets/NamePropertyWidget.ui"
    "Widgets/PropertiesWidgets/ReferencePropertyWidget.ui"
    "Widgets/PropertiesWidgets/Vec3PropertyWidget.ui"
    "Widgets/SelectObjectsDialog.ui"
    "Widgets/StatisticWidget.ui"
    )

set(SOURCES
    "AsyncTasks/AbstractAsyncTask.cpp"
    "AsyncTasks/AsyncTaskQueue.cpp"
    "AsyncTasks/SaveToFileTask.cpp"

    "Clipboard/CopyToClipboardOperation.cpp"
    "Clipboard/PasteFromClipboardOperation.cpp"

    "DrawModel/DrawModelAnimation.cpp"
    "DrawModel/DrawModelAnimationTrack.cpp"
    "DrawModel/DrawModelLoaderRegistry.cpp"
    "DrawModel/FbxModelLoader.cpp"
    "DrawModel/MasterDrawModel.cpp"
    "DrawModel/MeshControlNode.cpp"
    "DrawModel/MMDModelLoader.cpp"
    "DrawModel/SkinControlNode.cpp"
    "DrawModel/SlaveDrawModel.cpp"

    "EditCommands/AddObjectCommand.cpp"
    "EditCommands/CompositeCommand.cpp"
    "EditCommands/MoveObjectCommand.cpp"
    "EditCommands/RemoveObjectCommand.cpp"
    "EditCommands/UndoStack.cpp"

    "Manipulator/AbstractManipulator.cpp"
    "Manipulator/AxisMove3DManipulator.cpp"
    "Manipulator/Composite3DManipulator.cpp"
    "Manipulator/ManipulatorController.cpp"
    "Manipulator/PlaneMove3DManipulator.cpp"
    "Manipulator/RingRotation3DManipulator.cpp"
    "Manipulator/Simple3DManipulator.cpp"
    "Manipulator/UniformScale3DManipulator.cpp"

    "ResourceManager/CubeTextureLibrary.cpp"
    "ResourceManager/DrawModelLibrary.cpp"
    "ResourceManager/Texture2DLibrary.cpp"

    "Scene/Object.cpp"
    "Scene/ObjectFactory.cpp"
    "Scene/ObjectGroup.cpp"
    "Scene/ObjectItemListModel.cpp"
    "Scene/ObjectItemTreeModel.cpp"
    "Scene/ObjectLink.cpp"
    "Scene/ObjectLoader.cpp"
    "Scene/ObjectSaver.cpp"
    "Scene/ObjectVisitor.cpp"
    "Scene/Scene.cpp"

    "Widgets/Actions/CopyAction.cpp"
    "Widgets/Actions/DeleteAction.cpp"
    "Widgets/Actions/PasteAction.cpp"
    "Widgets/Actions/RedoAction.cpp"
    "Widgets/Actions/UndoAction.cpp"
    "Widgets/AsyncTaskDialog/AsyncTaskDialog.cpp"
    "Widgets/AsyncTaskDialog/TaskProgressWidget.cpp"
    "Widgets/PropertiesWidgets/Bitfield32Widget.cpp"
    "Widgets/PropertiesWidgets/ColorWidget.cpp"
    "Widgets/PropertiesWidgets/NamePropertyWidget.cpp"
    "Widgets/PropertiesWidgets/ReferenceWidget.cpp"
    "Widgets/PropertiesWidgets/Vec3Widget.cpp"
    "Widgets/RenderWidget/ClickSelectHandler.cpp"
    "Widgets/RenderWidget/OrbitalCameraController.cpp"
    "Widgets/RenderWidget/RenderWidget.cpp"
    "Widgets/ObjectsTreeView.cpp"
    "Widgets/SelectObjectsDialog.cpp"
    "Widgets/StatisticWidget.cpp"

    "WorkCycle/RenderImageAction.cpp"
    "WorkCycle/RenderSceneAction.cpp"
    "WorkCycle/WorkCycle.cpp"

    "Application.cpp"
    "ApplicationShaderLoader.cpp"
    "CommonEditData.cpp"
    )

add_library(application
            ${HEADERS}
            ${SOURCES}
            ${UI_FILES}
            )

target_link_libraries(application
                      Qt5::Core
                      Qt5::Widgets
                      )
